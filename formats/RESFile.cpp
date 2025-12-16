#include <algorithm>
#include <fstream>

#include "RESFile.h"

struct RESEntry
{
    uint32_t firstChildOffset; // for dirs (type != 0)
    uint32_t nextEntryOffset;
    uint32_t type;             // 0 == file
    uint32_t fileSize;         // for files
    uint32_t fileOffset;       // for files
};

static_assert(sizeof(RESEntry) == 20);

bool RESFile::load(std::string_view path)
{
    this->path = path;
    std::ifstream file(this->path, std::ios::binary);

    if(!file)
        return false;

    // get offset to file table
    uint32_t fileTableOffset = 0;
    file.read(reinterpret_cast<char *>(&fileTableOffset), 4);

    // read root dir
    parseEntries(file, fileTableOffset);

    return true;
}

std::optional<std::vector<uint8_t>> RESFile::getFileContents(std::string_view path)
{
    // find entry
    auto it = entries.find(path);

    if(it == entries.end())
    {
        // retry with lower case name
        auto lcPath = std::string(path);
        std::transform(lcPath.begin(), lcPath.end(), lcPath.begin(), ::tolower);

        auto lcIt = lowercaseNames.find(lcPath);
        if(lcIt == lowercaseNames.end())
            return {};

        // shouldn't fail if there was a mapping entry
        it = entries.find(lcIt->second);
    }

    // open file
    std::ifstream file(this->path, std::ios::binary);

    if(!file)
        return {};

    // resize buffer to size
    std::vector<uint8_t> ret;
    ret.resize(it->second.size);

    // read data
    file.seekg(it->second.offset);
    file.read(reinterpret_cast<char *>(ret.data()), ret.size());

    return ret;
}

void RESFile::parseEntries(std::ifstream &file, uint32_t rootOffset, uint32_t offset, std::string_view dir)
{
    file.seekg(rootOffset + offset);

    while(true)
    {
        RESEntry entry;
        file.read(reinterpret_cast<char *>(&entry), sizeof(entry));

        // read name
        std::string name;

        while(true)
        {
            char c = file.get();

            if(!c)
                break;

            name += c;
        }

        auto path = std::string(dir).append(name);

        // add to list and recurse if dir
        if(entry.type == 0) // file
            entries.emplace(path, Entry{entry.fileOffset, entry.fileSize});
        else // dir
        {
            path.append("/");
            entries.emplace(path, Entry{0, 0});
            parseEntries(file, rootOffset, entry.firstChildOffset, path);
        }

        // create mapping for case-insensitive lookup
        auto lcPath = path;
        std::transform(lcPath.begin(), lcPath.end(), lcPath.begin(), ::tolower);
        if(lcPath != path)
            lowercaseNames.emplace(lcPath, path);

        // no more entries
        if(entry.nextEntryOffset == 0xFFFFFFFF)
            break;

        // seek to next entry
        file.seekg(rootOffset + entry.nextEntryOffset);
    }
}