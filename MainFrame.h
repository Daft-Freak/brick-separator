#pragma once

#include "BrickSeparatorBase.h"
#include "FileSystem.h"

class MainFrame final : public MainFrameBase
{
public:
    MainFrame();

private:
    // events
    void onOpenFolder(wxCommandEvent &event) override;
    void onFileSelectionChanged(wxTreeListEvent &event) override;

    void buildFileList(std::filesystem::path path, wxTreeListItem parent);

    FileSystem fs;
};