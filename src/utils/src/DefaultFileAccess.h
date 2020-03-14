#pragma once

#include "FileAccess.h"
#include "FileExtensionsFilter.h"
#include "FilenamePathFilter.h"

namespace utils
{
class DefaultFileAccess : public FileAccess
{
public:
    void write(const std::string& absolutePath, const std::string& content) const override;
    void append(const std::string& absolutePath, const std::string& content) const override;
    std::string readContent(const std::string& absolutePath) const override;
    void createDirectory(const std::string& absolutePath) const override;
    void remove(const std::string& absolutePath) const override;
    void rename(const std::string& absolutePath, const std::string& newAbsolutePath) const override;
    bool exists(const std::string& absolutePath) const override;
    std::vector<std::string> getDirectoryFilePaths(const std::string& absolutePath) const override;
    //    std::vector<std::string> getFilteredDirectoryFilepaths(const std::string& absolutePath, const
    //    std::vector<std::string>& extensions) const override; std::vector<std::string>
    //    getDirectoryFilenames(const std::string& absolutePath) const override;
    std::vector<std::string> getDirectoryFilenames(const std::string& absolutePath,
                                                   const std::vector<std::string>& extensions) const override;

private:
    FilenamePathFilter filenamePathFilter;
    FileExtensionsFilter fileExtensionsFilter;
};
}