#include <set>

#include "RFDMount.h"

RFDMount::RFDMount(std::string_view resPath)
{
    file.load(resPath);
}

std::optional<std::vector<uint8_t>> RFDMount::getFileContents(std::string_view path)
{
    return file.getFileContents(path);
}

std::vector<FileEntry> RFDMount::listFiles(std::string_view path)
{
    auto &entries = file.getEntries();

    std::vector<FileEntry> ret;

    // RFD files don't list the directories
    std::set<std::string> dirs;

    for(auto &entry : entries)
    {
        // check prefix
        if(entry.first.compare(0, path.size(), path) != 0)
            continue;

        // make sure this is a leaf file/dir
        auto slashPos = entry.first.find_first_of('/', path.size());

        if(slashPos != std::string::npos)
        {
            // check parent dir
            auto entryDir = entry.first.substr(path.size(), slashPos - path.size());

            if(!entryDir.empty() && !dirs.count(entryDir))
            {
                dirs.emplace(entryDir);
                FileEntry retEntry;
                retEntry.name = entryDir;
                retEntry.size = 0;
                retEntry.isDir = true; 

                ret.emplace_back(retEntry);
            }

            continue;
        }

        // build entry
        FileEntry retEntry;
        retEntry.name = entry.first.substr(path.size());
        retEntry.size = entry.second.size;
        retEntry.isDir = false; 

        ret.emplace_back(retEntry);
    }

    return ret;
}
