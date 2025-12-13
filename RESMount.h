#pragma once
#include "FileSystem.h"
#include "RESFile.h"

class RESMount final : public FileSystemMount
{
public:
    RESMount(std::string_view resPath);

    std::optional<std::vector<uint8_t>> getFileContents(std::string_view path) override;
    std::vector<FileEntry> listFiles(std::string_view path) override;

    // files are embedded
    std::string getRealPath(std::string_view path) override {return "";}

private:
    RESFile file;
};
