#pragma once
#include "FileSystem.h"

#include "formats/RFDFile.h"

class RFDMount final : public FileSystemMount
{
public:
    RFDMount(std::string_view resPath);

    std::optional<std::vector<uint8_t>> getFileContents(std::string_view path) override;
    std::vector<FileEntry> listFiles(std::string_view path) override;

    // files are embedded
    std::string getRealPath(std::string_view path) override {return "";}

private:
    RFDFile file;
};
