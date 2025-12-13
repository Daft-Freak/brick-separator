#pragma once
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

// legoland resource file
class RESFile final
{
public:
    struct Entry
    {
        uint32_t offset;
        uint32_t size;
    };

    bool load(std::string_view path);

    std::optional<std::vector<uint8_t>> getFileContents(std::string_view path);

    const std::map<std::string, Entry, std::less<>> &getEntries() const {return entries;}

private:
    void parseEntries(std::ifstream &file, uint32_t rootOffset, uint32_t offset = 0, std::string_view dir = "");

    std::string path;

    std::map<std::string, Entry, std::less<>> entries;
};