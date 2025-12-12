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
        Directory =  0,
        Text,
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