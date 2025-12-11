#include <iostream>

#include <wx/dirdlg.h>

#include "MainFrame.h"
#include "FileSystem.h"

class FileListComparator final : public wxTreeListItemComparator
{
    int Compare(wxTreeListCtrl *treelist, unsigned column, wxTreeListItem first, wxTreeListItem second) override
    {
        auto firstText = treelist->GetItemText(first, column);
        auto secondText = treelist->GetItemText(second, column);

        // if not the size column, do a string compare
        if(column != 2)
            return firstText.compare(secondText);

        long firstV = 0, secondV = 0;

        // fall back to 0 if failed
        firstText.ToLong(&firstV);
        secondText.ToLong(&secondV);

        return firstV - secondV;
    }
};

static FileListComparator fileListComp;

MainFrame::MainFrame() : MainFrameBase(nullptr)
{
    // set custom comparator
    fileTree->SetItemComparator(&fileListComp);

    // default to sorting by name
    fileTree->SetSortColumn(0);

    buildFileList("", fileTree->GetRootItem());
}

void MainFrame::onOpenFolder(wxCommandEvent& event)
{
    wxDirDialog dirDialog(this, "Choose folder", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if(dirDialog.ShowModal() == wxID_OK)
    {
        // clear old mounts/items
        fs.unmountAll();
        fileTree->DeleteAllItems();

        // mount new dir
        fs.mount(std::make_shared<DirectoryMount>(dirDialog.GetPath().ToStdString()), "");

        // rebuild
        buildFileList("", fileTree->GetRootItem());
    }
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