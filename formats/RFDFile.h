#pragma once
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

// loco rfd/rfh file
class RFDFile final
{
public:
    struct Entry
    {
        uint32_t offset;
        uint32_t size;
        uint32_t flags;
    };

    bool load(std::string_view path);

    std::optional<std::vector<uint8_t>> getFileContents(std::string_view path);

    const std::map<std::string, Entry, std::less<>> &getEntries() const {return entries;}

private:
    std::string path;

    std::map<std::string, Entry, std::less<>> entries;
};