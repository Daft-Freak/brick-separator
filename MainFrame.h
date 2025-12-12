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
        Unknown = -1,
        Text    =  0,
    };

    // events
    void onOpenFolder(wxCommandEvent &event) override;
    void onFileSelectionChanged(wxTreeListEvent &event) override;

    void buildFileList(std::filesystem::path path, wxTreeListItem parent);
    FileType identifyFile(std::string path);

    FileSystem fs;

    wxPanel *curInfoPanel;
};