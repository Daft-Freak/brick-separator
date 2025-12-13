#include <iostream>

#include <wx/dirdlg.h>
#include <wx/mstream.h>

#include "MainFrame.h"

#include "FileSystem.h"
#include "InfoPanel.h"
#include "LLSInfoPanel.h"

class FileListComparator final : public wxTreeListItemComparator
{
    int Compare(wxTreeListCtrl *treelist, unsigned column, wxTreeListItem first, wxTreeListItem second) override
    {
        auto firstText = treelist->GetItemText(first, column);
        auto secondText = treelist->GetItemText(second, column);

        // if not the size column, do a string compare
        if(column != 2)
            return firstText.CmpNoCase(secondText);

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

    curInfoPanel = defaultInfoPanel;
}

void MainFrame::onOpenFolder(wxCommandEvent &event)
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

void MainFrame::onFileSelectionChanged(wxTreeListEvent &event)
{
    // build path from tree
    auto item = event.GetItem();

    auto path = fileTree->GetItemText(item);

    auto parent = fileTree->GetItemParent(item);
    while(parent != fileTree->GetRootItem())
    {
        path = fileTree->GetItemText(parent) + "/" + path;
        parent = fileTree->GetItemParent(parent);
    }

    auto type = identifyFile(path.ToStdString());

    wxPanel *newInfoPanel = nullptr;

    // update info panel
    switch(type)
    {
        case FileType::Text:
        {
            auto textPanel = new TextInfoPanel(this);
            newInfoPanel = textPanel;

            auto contents = fs.getFileContents(path.ToStdString());

            if(contents)
            {
                wxString contentsStr(contents->data(), contents->size());
                textPanel->setText(contentsStr);
            }
            else
                textPanel->setText("Failed to read...");
            break;
        }

        case FileType::Image:
        {
            auto imagePanel = new ImageInfoPanel(this);
            newInfoPanel = imagePanel;

            auto contents = fs.getFileContents(path.ToStdString());
            wxImage image;

            if(contents)
            {
                wxMemoryInputStream stream(contents->data(), contents->size());
                image.LoadFile(stream);
            }

            imagePanel->setImage(image);

            break;
        }

        case FileType::LLSprite:
        {
            auto spritePanel = new LLSInfoPanel(this);
            newInfoPanel = spritePanel;

            auto contents = fs.getFileContents(path.ToStdString());
            wxImage image;

            if(contents)
                spritePanel->loadSprite(contents->data(), contents->size());
            else
                spritePanel->loadSprite(nullptr, 0);

            break;
        }

        case FileType::Unknown:
        default:
        {
            // fallback panel with not so helpful generic info
            newInfoPanel = defaultInfoPanel;
            defaultInfoPanel->Show();

            wxString label;
            auto size = fileTree->GetItemText(item, 2);
            
            // empty size text is a dir
            if(size.empty())
                label = wxString::Format("%s\nfolder", path, size);
            else
                label = wxString::Format("%s\n%s byte file", path, size);

            infoLabel->SetLabel(label);
        }
    }

    // switch panels
    GetSizer()->Replace(curInfoPanel, newInfoPanel);

    // clean up non-default panel
    if(curInfoPanel != defaultInfoPanel)
        delete curInfoPanel;
    else if(newInfoPanel != curInfoPanel)
        curInfoPanel->Hide();

    curInfoPanel = newInfoPanel;

    Layout();
}

void MainFrame::buildFileList(std::filesystem::path path, wxTreeListItem parent)
{
    auto files = fs.listFiles(path.generic_string() + "/");

    for(auto &file : files)
    {
        auto itemPath = path / file.name;
        auto newItem = fileTree->AppendItem(parent, file.name);

        // default type (updated below if not a dir)
        auto type = FileType::Directory;

        // show size and try to identify type if not a dir
        if(!file.isDir)
        {
            type = identifyFile(itemPath);
            fileTree->SetItemText(newItem, 2, std::to_string(file.size));
        }

        // type column
        // display ext if not known
        if(type == FileType::Unknown)
            fileTree->SetItemText(newItem, 1, itemPath.extension().c_str() + 1);
        else
            fileTree->SetItemText(newItem, 1, getFileTypeLabel(type));

        // recurse
        if(file.isDir)
            buildFileList(itemPath, newItem);
    }
}

MainFrame::FileType MainFrame::identifyFile(std::string path)
{
    // get extension
    auto ext = std::filesystem::path(path).extension().string();
    // lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if(ext == ".txt")
        return FileType::Text;
    if(ext == ".bmp" || ext == ".png")
        return FileType::Image;

    if(ext == ".lls")
        return FileType::LLSprite;

    return FileType::Unknown;
}

std::string MainFrame::getFileTypeLabel(FileType type)
{
    switch(type)
    {
        case FileType::Text:
            return "text";

        case FileType::Image:
            return "image";

        case FileType::LLSprite:
            return "LL sprite";

        default:
            return "";
    }
}