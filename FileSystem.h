#pragma once
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string_view>
#include <optional>
#include <string>
#include <vector>

class FileEntry
{
public:
    std::string name;
    size_t size;
    bool isDir;
};

// mount interface
class FileSystemMount
{
public:
    virtual std::optional<std::vector<uint8_t>> getFileContents(std::string_view path) = 0;
    virtual std::vector<FileEntry> listFiles(std::string_view path) = 0;
};

// mounts a directory to the filesystem
class DirectoryMount final : public FileSystemMount
{
public:
    DirectoryMount(std::string basePath);

    std::optional<std::vector<uint8_t>> getFileContents(std::string_view path) override;
    std::vector<FileEntry> listFiles(std::string_view path) override;

private:
    std::filesystem::path basePath;
};


class FileSystem
{
public:
    std::optional<std::vector<uint8_t>> getFileContents(std::string_view path);
    std::vector<FileEntry> listFiles(std::string_view path);

    void mount(std::shared_ptr<FileSystemMount> mount, std::string mountPath);

private:
    std::tuple<std::shared_ptr<FileSystemMount>, std::string_view> findMount(std::string_view path);

    // reverse order so longer paths are first
    std::map<std::string, std::shared_ptr<FileSystemMount>, std::greater<std::string>> mounts;
};

// listFiles should return some additional data...