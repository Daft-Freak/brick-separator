#include <iostream>

#include "MainFrame.h"
#include "FileSystem.h"

MainFrame::MainFrame() : MainFrameBase(nullptr)
{
    buildFileList("", fileTree->GetRootItem());
}

void MainFrame::buildFileList(std::filesystem::path path, wxTreeListItem parent)
{
    auto files = fs.listFiles(path.generic_string() + "/");

    for(auto &file : files)
    {
        auto itemPath = path / file.name;
        auto newItem = fileTree->AppendItem(parent, file.name);

        // "type" column (not too useful yet)
        if(itemPath.has_extension())
            fileTree->SetItemText(newItem, 1, itemPath.extension().c_str() + 1);

        if(file.size)
            fileTree->SetItemText(newItem, 2, std::to_string(file.size));

        // recurse
        if(file.isDir)
            buildFileList(itemPath, newItem);
    }
}