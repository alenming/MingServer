#ifndef ___FILEUTILS_H__
#define ___FILEUTILS_H__

#include <type_traits>
#include "Value.h"
#include "Data.h"


class ResizableBuffer {
public:
    virtual ~ResizableBuffer() {}
    virtual void resize(size_t size) = 0;
    virtual void* buffer() const = 0;
};

template<typename T>
class ResizableBufferAdapter { };

template<typename CharT, typename Traits, typename Allocator>
class ResizableBufferAdapter< std::basic_string<CharT, Traits, Allocator> > : public ResizableBuffer 
{
    typedef std::basic_string<CharT, Traits, Allocator> BufferType;
    BufferType* _buffer;
	public:
    explicit ResizableBufferAdapter(BufferType* buffer):
	_buffer(buffer)
	{
	}
    virtual void resize(size_t size) override 
	{
        _buffer->resize((size + sizeof(CharT) - 1) / sizeof(CharT));
    }
    virtual void* buffer() const override
	{
		if (_buffer->empty())
		{
			return nullptr;
		}
		else
		{
			return &_buffer->front();
		}    
    }
};

template<typename T, typename Allocator>
class ResizableBufferAdapter< std::vector<T, Allocator> > : public ResizableBuffer
{
    typedef std::vector<T, Allocator> BufferType;
    BufferType* _buffer;
public:
    explicit ResizableBufferAdapter(BufferType* buffer) : _buffer(buffer)
	{
	}
    virtual void resize(size_t size) override
	{
        _buffer->resize((size + sizeof(T) - 1) / sizeof(T));
    }
    virtual void* buffer() const override
	{
        if (_buffer->empty())
            return nullptr;
        else
            return &_buffer->front();
    }
};


template<>
class ResizableBufferAdapter<Data> : public ResizableBuffer
{
    typedef Data BufferType;
    BufferType* _buffer;
public:
    explicit ResizableBufferAdapter(BufferType* buffer) : _buffer(buffer) {}
    virtual void resize(size_t size) override
	{
        if (static_cast<size_t>(_buffer->getSize()) < size)
		{
            auto old = _buffer->getBytes();
            void* buffer = realloc(old, size);
            if (buffer)
                _buffer->fastSet((unsigned char*)buffer, size);
        }
    }
    virtual void* buffer() const override 
	{
        return _buffer->getBytes();
    }
};

class FileUtils
{
public:
    static FileUtils* getInstance();
    static void destroyInstance();

    static void setDelegate(FileUtils *delegate);
    static FileUtils* sharedFileUtils() { return getInstance(); }

    static void purgeFileUtils() { destroyInstance(); }
    virtual ~FileUtils();

    virtual void purgeCachedEntries();

    virtual std::string getStringFromFile(const std::string& filename);

    virtual Data getDataFromFile(const std::string& filename);


    enum class Status
    {
        OK = 0,
        NotExists = 1, // File not exists
        OpenFailed = 2, // Open file failed.
        ReadFailed = 3, // Read failed
        NotInitialized = 4, // FileUtils is not initializes
        TooLarge = 5, // The file is too large (great than 2^32-1)
        ObtainSizeFailed = 6 // Failed to obtain the file size.
    };
    template <
        typename T,
        typename Enable = typename std::enable_if<
            std::is_base_of< ResizableBuffer, ResizableBufferAdapter<T> >::value
        >::type
    >
    Status getContents(const std::string& filename, T* buffer) {
        ResizableBufferAdapter<T> buf(buffer);
        return getContents(filename, &buf);
    }
    virtual Status getContents(const std::string& filename, ResizableBuffer* buffer);

    virtual unsigned char* getFileData(const std::string& filename, const char* mode, size_t *size);

    virtual unsigned char* getFileDataFromZip(const std::string& zipFilePath, const std::string& filename, size_t *size);

    virtual std::string fullPathForFilename(const std::string &filename) const;

  
    virtual void loadFilenameLookupDictionaryFromFile(const std::string &filename);
    virtual void setFilenameLookupDictionary(const ValueMap& filenameLookupDict);
    virtual std::string fullPathFromRelativeFile(const std::string &filename, const std::string &relativeFile);
    virtual void setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder);
    virtual void addSearchResolutionsOrder(const std::string &order,const bool front=false);
    virtual const std::vector<std::string>& getSearchResolutionsOrder() const;
    virtual void setSearchPaths(const std::vector<std::string>& searchPaths);
    void setDefaultResourceRootPath(const std::string& path);
    void addSearchPath(const std::string & path, const bool front=false);
    virtual const std::vector<std::string>& getSearchPaths() const;
    virtual std::string getWritablePath() const = 0;
    virtual void setWritablePath(const std::string& writablePath);
    virtual void setPopupNotify(bool notify);
    virtual bool isPopupNotify() const;
    virtual ValueMap getValueMapFromFile(const std::string& filename);
    virtual ValueMap getValueMapFromData(const char* filedata, int filesize);
    virtual bool writeToFile(const ValueMap& dict, const std::string& fullPath);
    virtual bool writeStringToFile(const std::string& dataStr, const std::string& fullPath);
    virtual bool writeDataToFile(const Data& data, const std::string& fullPath);
	virtual bool writeValueMapToFile(const ValueMap& dict, const std::string& fullPath){return true;}
	virtual bool writeValueVectorToFile(const ValueVector& vecData, const std::string& fullPath){return true;}
    virtual std::string getSuitableFOpen(const std::string& filenameUtf8) const;
    virtual ValueVector getValueVectorFromFile(const std::string& filename);
    virtual bool isFileExist(const std::string& filename) const;
    virtual std::string getFileExtension(const std::string& filePath) const;
    virtual bool isAbsolutePath(const std::string& path) const;
    virtual bool isDirectoryExist(const std::string& dirPath) const;
    virtual bool createDirectory(const std::string& dirPath);
    virtual bool removeDirectory(const std::string& dirPath);
    virtual bool removeFile(const std::string &filepath);
    virtual bool renameFile(const std::string &path, const std::string &oldname, const std::string &name);
    virtual bool renameFile(const std::string &oldfullpath, const std::string &newfullpath);
    virtual long getFileSize(const std::string &filepath);
    const std::unordered_map<std::string, std::string>& getFullPathCache() const { return _fullPathCache; }

protected:

    FileUtils();
    virtual bool init();
    virtual std::string getNewFilename(const std::string &filename) const;
    virtual bool isFileExistInternal(const std::string& filename) const = 0;
    virtual bool isDirectoryExistInternal(const std::string& dirPath) const;
    virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const;
    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const;
    ValueMap _filenameLookupDict;
    std::vector<std::string> _searchResolutionsOrderArray;
    std::vector<std::string> _searchPathArray;
    std::string _defaultResRootPath;
    mutable std::unordered_map<std::string, std::string> _fullPathCache;
    std::string _writablePath;
    static FileUtils* s_sharedFileUtils;
    virtual void valueMapCompact(ValueMap& valueMap);
    virtual void valueVectorCompact(ValueVector& valueVector);
};

#endif    // ___FILEUTILS_H__
