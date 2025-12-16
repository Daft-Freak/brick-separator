#include "RESMount.h"

RESMount::RESMount(std::string_view resPath)
{
    file.load(resPath);
}

std::optional<std::vector<uint8_t>> RESMount::getFileContents(std::string_view path)
{
    return file.getFileContents(path);
}

std::vector<FileEntry> RESMount::listFiles(std::string_view path)
{
    auto &entries = file.getEntries();

    std::vector<FileEntry> ret;

    for(auto &entry : entries)
    {
        // check prefix
        if(entry.first.compare(0, path.size(), path) != 0)
            continue;

        // skip the dir itself (we want the children)
        if(entry.first == path)
            continue;

        // make sure this is a leaf file/dir
        auto slashPos = entry.first.find_first_of('/', path.size());

        if(slashPos != std::string::npos && slashPos < entry.first.size() - 1)
            continue;

        // build entry
        FileEntry retEntry;
        retEntry.name = entry.first.substr(path.size());
        retEntry.size = entry.second.size;
        retEntry.isDir = entry.second.size == 0 && entry.first.back() == '/'; 

        // trim trailing slash
        if(retEntry.name.back() == '/')
            retEntry.name.pop_back();

        ret.emplace_back(retEntry);
    }

    return ret;
}
