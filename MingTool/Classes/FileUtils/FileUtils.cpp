#include "FileUtils.h"
#include <stack>
#include <sys/stat.h>
#include "Common/CommonHead.h"

ValueMap FileUtils::getValueMapFromFile(const std::string& filename) {return ValueMap();}
ValueMap FileUtils::getValueMapFromData(const char* filedata, int filesize) {return ValueMap();}
ValueVector FileUtils::getValueVectorFromFile(const std::string& filename) {return ValueVector();}
bool FileUtils::writeToFile(const ValueMap& dict, const std::string &fullPath) {return false;}

FileUtils* FileUtils::s_sharedFileUtils = nullptr;

void FileUtils::destroyInstance()
{
    MING_SAFE_DELETE(s_sharedFileUtils);
}

void FileUtils::setDelegate(FileUtils *delegate)
{
    if (s_sharedFileUtils)
        delete s_sharedFileUtils;

    s_sharedFileUtils = delegate;
}

FileUtils::FileUtils()
    : _writablePath("")
{
}

FileUtils::~FileUtils()
{
}

bool FileUtils::writeStringToFile(const std::string& dataStr, const std::string& fullPath)
{
    Data data;
    data.fastSet((unsigned char*)dataStr.c_str(), dataStr.size());

    bool rv = writeDataToFile(data, fullPath);

    data.fastSet(nullptr, 0);
    return rv;
}

bool FileUtils::writeDataToFile(const Data& data, const std::string& fullPath)
{
    size_t size = 0;
    const char* mode = "wb";

    MINGASSERT(!fullPath.empty() && data.getSize() != 0, "Invalid parameters.");

    auto fileutils = FileUtils::getInstance();
    do
    {
        // Read the file from hardware
        FILE *fp = fopen(fileutils->getSuitableFOpen(fullPath).c_str(), mode);
        MING_BREAK_IF(!fp);
        size = data.getSize();

        fwrite(data.getBytes(), size, 1, fp);

        fclose(fp);

        return true;
    } while (0);

    return false;
}

bool FileUtils::init()
{
    _searchPathArray.push_back(_defaultResRootPath);
    _searchResolutionsOrderArray.push_back("");
    return true;
}

void FileUtils::purgeCachedEntries()
{
    _fullPathCache.clear();
}

std::string FileUtils::getStringFromFile(const std::string& filename)
{
    std::string s;
    getContents(filename, &s);
    return s=="" ? "file no find!" : s;
}

Data FileUtils::getDataFromFile(const std::string& filename)
{
    Data d;
    getContents(filename, &d);
	return d;
}


FileUtils::Status FileUtils::getContents(const std::string& filename, ResizableBuffer* buffer)
{
    if (filename.empty())
        return Status::NotExists;

    auto fs = FileUtils::getInstance();

    std::string fullPath = fs->fullPathForFilename(filename);
    if (fullPath.empty())
        return Status::NotExists;

    FILE *fp = fopen(fs->getSuitableFOpen(fullPath).c_str(), "rb");
    if (!fp)
        return Status::OpenFailed;

#if defined(_MSC_VER)
    auto descriptor = _fileno(fp);
#else
    auto descriptor = fileno(fp);
#endif
    struct stat statBuf;
    if (fstat(descriptor, &statBuf) == -1) {
        fclose(fp);
        return Status::ReadFailed;
    }
    size_t size = statBuf.st_size;

    buffer->resize(size);
    size_t readsize = fread(buffer->buffer(), 1, size, fp);
    fclose(fp);

    if (readsize < size) {
        buffer->resize(readsize);
        return Status::ReadFailed;
    }

    return Status::OK;
}

unsigned char* FileUtils::getFileData(const std::string& filename, const char* mode, size_t *size)
{
    MINGASSERT(!filename.empty() && size != nullptr && mode != nullptr, "Invalid parameters.");
    (void)(mode); // mode is unused, as we do not support text mode any more...

    Data d;
    if (getContents(filename, &d) != Status::OK) {
        *size = 0;
        return nullptr;
    }

    return d.takeBuffer(size);
}

unsigned char* FileUtils::getFileDataFromZip(const std::string& zipFilePath, const std::string& filename, size_t *size)
{
    unsigned char * buffer = nullptr;
    return buffer;
}

std::string FileUtils::getNewFilename(const std::string &filename) const
{
    std::string newFileName;

    // in Lookup Filename dictionary ?
    auto iter = _filenameLookupDict.find(filename);

    if (iter == _filenameLookupDict.end())
    {
        newFileName = filename;
    }
    else
    {
        newFileName = iter->second.asString();
    }
    return newFileName;
}

std::string FileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const
{
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }

    // searchPath + file_path + resourceDirectory
    std::string path = searchPath;
    path += file_path;
    path += resolutionDirectory;

    path = getFullPathForDirectoryAndFilename(path, file);

    return path;
}

std::string FileUtils::fullPathForFilename(const std::string &filename) const
{
    if (filename.empty())
    {
        return "";
    }

    if (isAbsolutePath(filename))
    {
        return filename;
    }

    // Already Cached ?
    auto cacheIter = _fullPathCache.find(filename);
    if(cacheIter != _fullPathCache.end())
    {
        return cacheIter->second;
    }

    // Get the new file name.
    const std::string newFilename( getNewFilename(filename) );

    std::string fullpath;

    for (const auto& searchIt : _searchPathArray)
    {
        for (const auto& resolutionIt : _searchResolutionsOrderArray)
        {
            fullpath = this->getPathForFilename(newFilename, resolutionIt, searchIt);

            if (!fullpath.empty())
            {
                // Using the filename passed in as key.
                _fullPathCache.insert(std::make_pair(filename, fullpath));
                return fullpath;
            }

        }
    }

    if(isPopupNotify()){
        MINGLOG("cocos2d: fullPathForFilename: No file found at %s. Possible missing file.", filename.c_str());
    }

    // The file wasn't found, return empty string.
    return "";
}

std::string FileUtils::fullPathFromRelativeFile(const std::string &filename, const std::string &relativeFile)
{
    return relativeFile.substr(0, relativeFile.rfind('/')+1) + getNewFilename(filename);
}

void FileUtils::setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder)
{
    bool existDefault = false;
    _fullPathCache.clear();
    _searchResolutionsOrderArray.clear();
    for(const auto& iter : searchResolutionsOrder)
    {
        std::string resolutionDirectory = iter;
        if (!existDefault && resolutionDirectory == "")
        {
            existDefault = true;
        }

        if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != '/')
        {
            resolutionDirectory += "/";
        }

        _searchResolutionsOrderArray.push_back(resolutionDirectory);
    }

    if (!existDefault)
    {
        _searchResolutionsOrderArray.push_back("");
    }
}

void FileUtils::addSearchResolutionsOrder(const std::string &order,const bool front)
{
    std::string resOrder = order;
    if (!resOrder.empty() && resOrder[resOrder.length()-1] != '/')
        resOrder.append("/");

    if (front) {
        _searchResolutionsOrderArray.insert(_searchResolutionsOrderArray.begin(), resOrder);
    } else {
        _searchResolutionsOrderArray.push_back(resOrder);
    }
}

const std::vector<std::string>& FileUtils::getSearchResolutionsOrder() const
{
    return _searchResolutionsOrderArray;
}

const std::vector<std::string>& FileUtils::getSearchPaths() const
{
    return _searchPathArray;
}

void FileUtils::setWritablePath(const std::string& writablePath)
{
    _writablePath = writablePath;
}

void FileUtils::setDefaultResourceRootPath(const std::string& path)
{
    _defaultResRootPath = path;
}

void FileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool existDefaultRootPath = false;

    _fullPathCache.clear();
    _searchPathArray.clear();
    for (const auto& iter : searchPaths)
    {
        std::string prefix;
        std::string path;

        if (!isAbsolutePath(iter))
        { // Not an absolute path
            prefix = _defaultResRootPath;
        }
        path = prefix + (iter);
        if (!path.empty() && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!existDefaultRootPath && path == _defaultResRootPath)
        {
            existDefaultRootPath = true;
        }
        _searchPathArray.push_back(path);
    }

    if (!existDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        _searchPathArray.push_back(_defaultResRootPath);
    }
}

void FileUtils::addSearchPath(const std::string &searchpath,const bool front)
{
    std::string prefix;
    if (!isAbsolutePath(searchpath))
        prefix = _defaultResRootPath;

    std::string path = prefix + searchpath;
    if (!path.empty() && path[path.length()-1] != '/')
    {
        path += "/";
    }
    if (front) {
        _searchPathArray.insert(_searchPathArray.begin(), path);
    } else {
        _searchPathArray.push_back(path);
    }
}

void FileUtils::setFilenameLookupDictionary(const ValueMap& filenameLookupDict)
{
    _fullPathCache.clear();
    _filenameLookupDict = filenameLookupDict;
}

void FileUtils::loadFilenameLookupDictionaryFromFile(const std::string &filename)
{
    const std::string fullPath = fullPathForFilename(filename);
    if (!fullPath.empty())
    {
        ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
        if (!dict.empty())
        {
            ValueMap& metadata =  dict["metadata"].asValueMap();
            int version = metadata["version"].asInt();
            if (version != 1)
            {
                MINGLOG("cocos2d: ERROR: Invalid filenameLookup dictionary version: %d. Filename: %s", version, filename.c_str());
                return;
            }
            setFilenameLookupDictionary( dict["filenames"].asValueMap());
        }
    }
}

std::string FileUtils::getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const
{
    // get directory+filename, safely adding '/' as necessary
    std::string ret = directory;
    if (directory.size() && directory[directory.size()-1] != '/'){
        ret += '/';
    }
    ret += filename;

    // if the file doesn't exist, return an empty string
    if (!isFileExistInternal(ret)) {
        ret = "";
    }
    return ret;
}

bool FileUtils::isFileExist(const std::string& filename) const
{
    if (isAbsolutePath(filename))
    {
        return isFileExistInternal(filename);
    }
    else
    {
        std::string fullpath = fullPathForFilename(filename);
        if (fullpath.empty())
            return false;
        else
            return true;
    }
}

bool FileUtils::isAbsolutePath(const std::string& path) const
{
    return (path[0] == '/');
}

bool FileUtils::isDirectoryExist(const std::string& dirPath) const
{
    MINGASSERT(!dirPath.empty(), "Invalid path");

    if (isAbsolutePath(dirPath))
    {
        return isDirectoryExistInternal(dirPath);
    }

    // Already Cached ?
    auto cacheIter = _fullPathCache.find(dirPath);
    if( cacheIter != _fullPathCache.end() )
    {
        return isDirectoryExistInternal(cacheIter->second);
    }

    std::string fullpath;
    for (const auto& searchIt : _searchPathArray)
    {
        for (const auto& resolutionIt : _searchResolutionsOrderArray)
        {
            // searchPath + file_path + resourceDirectory
            fullpath = fullPathForFilename(searchIt + dirPath + resolutionIt);
            if (isDirectoryExistInternal(fullpath))
            {
                _fullPathCache.insert(std::make_pair(dirPath, fullpath));
                return true;
            }
        }
    }
    return false;
}

#if (MING_TARGET_PLATFORM == MING_PLATFORM_WIN32)
// windows os implement should override in platform specific FileUtiles class
bool FileUtils::isDirectoryExistInternal(const std::string& dirPath) const
{
	MINGASSERT(false, "FileUtils not support isDirectoryExistInternal");
    return false;
}

bool FileUtils::createDirectory(const std::string& path)
{
	MINGASSERT(false, "FileUtils not support createDirectory");
    return false;
}

bool FileUtils::removeDirectory(const std::string& path)
{
	MINGASSERT(false, "FileUtils not support removeDirectory");
    return false;
}

bool FileUtils::removeFile(const std::string &path)
{
	MINGASSERT(false, "FileUtils not support removeFile");
    return false;
}

bool FileUtils::renameFile(const std::string &oldfullpath, const std::string& newfullpath)
{
	MINGASSERT(false, "FileUtils not support renameFile");
    return false;
}

bool FileUtils::renameFile(const std::string &path, const std::string &oldname, const std::string &name)
{
	MINGASSERT(false, "FileUtils not support renameFile");
    return false;
}

std::string FileUtils::getSuitableFOpen(const std::string& filenameUtf8) const
{
	MINGASSERT(false, "getSuitableFOpen should be override by platform FileUtils");
    return filenameUtf8;
}

long FileUtils::getFileSize(const std::string &filepath)
{
	MINGASSERT(false, "getFileSize should be override by platform FileUtils");
    return 0;
}

#else
// default implements for unix like os
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

bool FileUtils::isDirectoryExistInternal(const std::string& dirPath) const
{
    struct stat st;
    if (stat(dirPath.c_str(), &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

bool FileUtils::createDirectory(const std::string& path)
{
    CCASSERT(!path.empty(), "Invalid path");

    if (isDirectoryExist(path))
        return true;

    // Split the path
    size_t start = 0;
    size_t found = path.find_first_of("/\\", start);
    std::string subpath;
    std::vector<std::string> dirs;

    if (found != std::string::npos)
    {
        while (true)
        {
            subpath = path.substr(start, found - start + 1);
            if (!subpath.empty())
                dirs.push_back(subpath);
            start = found+1;
            found = path.find_first_of("/\\", start);
            if (found == std::string::npos)
            {
                if (start < path.length())
                {
                    dirs.push_back(path.substr(start));
                }
                break;
            }
        }
    }

    DIR *dir = NULL;

    // Create path recursively
    subpath = "";
    for (int i = 0; i < dirs.size(); ++i)
    {
        subpath += dirs[i];
        dir = opendir(subpath.c_str());

        if (!dir)
        {
            // directory doesn't exist, should create a new one

            int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
            if (ret != 0 && (errno != EEXIST))
            {
                // current directory can not be created, sub directories can not be created too
                // should return
                return false;
            }
        }
        else
        {
            // directory exists, should close opened dir
            closedir(dir);
        }
    }
    return true;
}

bool FileUtils::removeDirectory(const std::string& path)
{
    std::string command = "rm -r ";
    // Path may include space.
    command += "\"" + path + "\"";
    if (system(command.c_str()) >= 0)
        return true;
    else
        return false;
}

bool FileUtils::removeFile(const std::string &path)
{
    if (remove(path.c_str())) {
        return false;
    } else {
        return true;
    }
}

bool FileUtils::renameFile(const std::string &oldfullpath, const std::string &newfullpath)
{
    CCASSERT(!oldfullpath.empty(), "Invalid path");
    CCASSERT(!newfullpath.empty(), "Invalid path");

    int errorCode = rename(oldfullpath.c_str(), newfullpath.c_str());

    if (0 != errorCode)
    {
        CCLOGERROR("Fail to rename file %s to %s !Error code is %d", oldfullpath.c_str(), newfullpath.c_str(), errorCode);
        return false;
    }
    return true;
}

bool FileUtils::renameFile(const std::string &path, const std::string &oldname, const std::string &name)
{
    CCASSERT(!path.empty(), "Invalid path");
    std::string oldPath = path + oldname;
    std::string newPath = path + name;

    return this->renameFile(oldPath, newPath);
}

std::string FileUtils::getSuitableFOpen(const std::string& filenameUtf8) const
{
    return filenameUtf8;
}


long FileUtils::getFileSize(const std::string &filepath)
{
    CCASSERT(!filepath.empty(), "Invalid path");

    std::string fullpath = filepath;
    if (!isAbsolutePath(filepath))
    {
        fullpath = fullPathForFilename(filepath);
        if (fullpath.empty())
            return 0;
    }

    struct stat info;
    // Get data associated with "crt_stat.c":
    int result = stat(fullpath.c_str(), &info);

    // Check if statistics are valid:
    if (result != 0)
    {
        // Failed
        return -1;
    }
    else
    {
        return (long)(info.st_size);
    }
}
#endif

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_popupNotify = true;

void FileUtils::setPopupNotify(bool notify)
{
    s_popupNotify = notify;
}

bool FileUtils::isPopupNotify() const
{
    return s_popupNotify;
}

std::string FileUtils::getFileExtension(const std::string& filePath) const
{
    std::string fileExtension;
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos)
    {
        fileExtension = filePath.substr(pos, filePath.length());

        std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
    }

    return fileExtension;
}

void FileUtils::valueMapCompact(ValueMap& valueMap)
{
}

void FileUtils::valueVectorCompact(ValueVector& valueVector)
{
}
