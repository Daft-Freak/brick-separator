#include <fstream>

#include "FileSystem.h"

DirectoryMount::DirectoryMount(std::string basePath) : basePath(basePath)
{}

std::optional<std::vector<uint8_t>> DirectoryMount::getFileContents(std::string_view path)
{
    auto fullPath = basePath / path;

    // read it
    std::vector<uint8_t> ret;

    std::ifstream file(fullPath, std::ios::binary);

    if(!file)
        return {};

    // resize buffer to size
    file.seekg(0, std::ios::end);
    ret.resize(file.tellg());

    // read data
    file.seekg(0);
    file.read(reinterpret_cast<char *>(ret.data()), ret.size());

    return ret;
}

std::vector<FileEntry> DirectoryMount::listFiles(std::string_view path)
{
    // avoid accidentally using the true fs root
    auto fullPath = path == "/" ? basePath : basePath / path;

    std::vector<FileEntry> ret;

    for(auto &entry : std::filesystem::directory_iterator(fullPath))
    {
        FileEntry retEntry;
        retEntry.name = entry.path().filename();
        retEntry.size = entry.is_directory() ? 0 : entry.file_size();
        retEntry.isDir = entry.is_directory();
        ret.emplace_back(retEntry);
    }
    return ret;
}

std::optional<std::vector<uint8_t>> FileSystem::getFileContents(std::string_view path)
{
    auto [mount, relPath] = findMount(path);
    if(!mount)
        return {};

    return mount->getFileContents(relPath);
}

std::vector<FileEntry> FileSystem::listFiles(std::string_view path)
{
    auto [mount, relPath] = findMount(path);
    if(!mount)
        return {};

    return mount->listFiles(relPath);
}

void FileSystem::mount(std::shared_ptr<FileSystemMount> mount, std::string mountPath)
{
    mounts.emplace(mountPath, mount);
}

std::tuple<std::shared_ptr<FileSystemMount>, std::string_view> FileSystem::findMount(std::string_view path)
{
    // find first mount with path that matches
    // return path relative to mount point
    for(auto &m : mounts)
    {
        if(path.compare(0, m.first.size(), m.first) == 0)
            return {m.second, path.substr(m.first.size())};
    }
    return {nullptr, ""};
}