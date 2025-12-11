#pragma once

#include "BrickSeparatorBase.h"
#include "FileSystem.h"

class MainFrame final : public MainFrameBase
{
public:
    MainFrame();

private:
    void buildFileList(std::filesystem::path path, wxTreeListItem parent);

    FileSystem fs;
};