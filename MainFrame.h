#pragma once

#include "BrickSeparatorBase.h"
#include "FileSystem.h"

class MainFrame final : public MainFrameBase
{
public:
    MainFrame();

private:
    enum class FileType
    {
        Unknown   = -1,
        // generic types
        Directory =  0,
        Text,
        Image,

        // legoland
        LLSprite,
    };

    // events
    void onOpenFolder(wxCommandEvent &event) override;
    void onFileSelectionChanged(wxTreeListEvent &event) override;

    void buildFileList(std::filesystem::path path, wxTreeListItem parent);

    FileType identifyFile(std::string path);
    std::string getFileTypeLabel(FileType type);

    FileSystem fs;

    wxPanel *curInfoPanel;
};