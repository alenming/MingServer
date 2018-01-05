#ifndef ___FILEUTILS_WIN32_H__
#define ___FILEUTILS_WIN32_H__

#if MING_TARGET_PLATFORM == MING_PLATFORM_WIN32

#include "FileUtils.h"


/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class FileUtilsWin32 : public FileUtils
{
    friend class FileUtils;
protected:
    FileUtilsWin32();
public:
    /* override functions */
    bool init();
    virtual std::string getWritablePath() const override;
    virtual bool isAbsolutePath(const std::string& strPath) const override;
    virtual std::string getSuitableFOpen(const std::string& filenameUtf8) const override;
    virtual long getFileSize(const std::string &filepath);
protected:

    virtual bool isFileExistInternal(const std::string& strFilePath) const override;

    virtual bool renameFile(const std::string &path, const std::string &oldname, const std::string &name) override;

    virtual bool renameFile(const std::string &oldfullpath, const std::string &newfullpath) override;

    virtual bool isDirectoryExistInternal(const std::string& dirPath) const override;

    virtual bool removeFile(const std::string &filepath) override;

    virtual bool createDirectory(const std::string& dirPath) override;

    virtual bool removeDirectory(const std::string& dirPath) override;

	virtual FileUtils::Status getContents(const std::string& filename, ResizableBuffer* buffer) override;

    virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const override;

    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const override;
};


#endif // MING_TARGET_PLATFORM == MING_PLATFORM_WIN32

#endif    // __CC_FILEUTILS_WIN32_H__
