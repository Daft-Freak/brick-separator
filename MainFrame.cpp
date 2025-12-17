#include <iostream>

#include <wx/dirdlg.h>
#include <wx/mstream.h>

#include "MainFrame.h"

#include "filesystem/FileSystem.h"
#include "filesystem/RESMount.h"
#include "filesystem/RFDMount.h"

#include "panels/CRDInfoPanel.h"
#include "panels/CSPInfoPanel.h"
#include "panels/InfoPanel.h"
#include "panels/LLSInfoPanel.h"
#include "panels/Model3DInfoPanel.h"

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

    auto data = static_cast<FileItemData *>(fileTree->GetItemData(item));
    auto path = data->path;
    auto type = data->type;

    wxPanel *newInfoPanel = nullptr;

    // update info panel
    switch(type)
    {
        case FileType::Text:
        {
            auto textPanel = new TextInfoPanel(wrapperPanel);
            newInfoPanel = textPanel;

            auto contents = fs.getFileContents(path);

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
            auto imagePanel = new ImageInfoPanel(wrapperPanel);
            newInfoPanel = imagePanel;

            auto contents = fs.getFileContents(path);
            wxImage image;

            if(contents)
            {
                wxMemoryInputStream stream(contents->data(), contents->size());
                image.LoadFile(stream);
            }

            imagePanel->setImage(image);

            break;
        }

        case FileType::LL3D:
        {
            auto panel3D = new Model3DInfoPanel(wrapperPanel, fs);
            newInfoPanel = panel3D;

            auto contents = fs.getFileContents(path);

            if(contents)
                panel3D->loadFile(contents->data(), contents->size(), path);
            else
                panel3D->loadFile(nullptr, 0, "");
            break;
        }

        case FileType::LLCompSprite:
        {
            auto cspPanel = new CSPInfoPanel(wrapperPanel, fs);
            newInfoPanel = cspPanel;

            auto contents = fs.getFileContents(path);

            if(contents)
                cspPanel->loadFile(contents->data(), contents->size());
            else
                cspPanel->loadFile(nullptr, 0);

            break;
        }

        case FileType::LLResource:
        {
            // display file list
            auto textPanel = new TextInfoPanel(wrapperPanel);
            newInfoPanel = textPanel;

            RESFile resFile;
            if(resFile.load(fs.getRealPath(path)))
            {
                wxString text;
                auto entries = resFile.getEntries();
                for(auto &ent : entries)
                {
                    // insert a blank line on dir change
                    if(!text.empty() && ent.first.back() == '/')
                        text << "\n";
                    text << ent.first << " offset " << ent.second.offset << " size " << ent.second.size << "\n";
                }
                textPanel->setText(text);
            }
            else
                textPanel->setText("Failed to read...");
            break;
        }

        case FileType::LLSprite:
        {
            auto spritePanel = new LLSInfoPanel(wrapperPanel);
            newInfoPanel = spritePanel;

            auto contents = fs.getFileContents(path);
            wxImage image;

            if(contents)
                spritePanel->loadSprite(contents->data(), contents->size());
            else
                spritePanel->loadSprite(nullptr, 0);

            break;
        }

        case FileType::LocoCard:
        {
            auto panel3D = new CRDInfoPanel(wrapperPanel, fs);
            newInfoPanel = panel3D;

            auto contents = fs.getFileContents(path);

            if(contents)
                panel3D->loadFile(contents->data(), contents->size());
            else
                panel3D->loadFile(nullptr, 0);
            break;
        }

        case FileType::LocoResource:
        {
            // display file list
            auto textPanel = new TextInfoPanel(wrapperPanel);
            newInfoPanel = textPanel;

            RFDFile resFile;
            if(resFile.load(fs.getRealPath(path)))
            {
                wxString text;
                auto entries = resFile.getEntries();
                for(auto &ent : entries)
                {
                    // insert a blank line on dir change
                    if(!text.empty() && ent.first.back() == '/')
                        text << "\n";
                    text << ent.first << " offset " << ent.second.offset << " size " << ent.second.size;
                    
                    if(ent.second.flags & 1)
                        text << " (compressed)";

                    text << "\n";
                }
                textPanel->setText(text);
            }
            else
                textPanel->setText("Failed to read...");
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
    wrapperPanel->GetSizer()->Replace(curInfoPanel, newInfoPanel);

    // clean up non-default panel
    if(curInfoPanel != defaultInfoPanel)
        delete curInfoPanel;
    else if(newInfoPanel != curInfoPanel)
        curInfoPanel->Hide();

    curInfoPanel = newInfoPanel;

    Layout();
}

void MainFrame::onFileActivated(wxTreeListEvent &event)
{
    auto item = event.GetItem();

    auto data = static_cast<FileItemData *>(fileTree->GetItemData(item));
    auto path = data->path;
    auto type = data->type;

    // if the item already has children, do nothing
    if(fileTree->GetFirstChild(item).IsOk())
        return;

    if(type == FileType::LLResource)
    {
        // mount the resource on top of itself
        auto resPath = fs.getRealPath(path);
        fs.mount(std::make_shared<RESMount>(resPath), path + "/");

        // update tree
        buildFileList(path, item);
    }
    else if(type == FileType::LocoResource)
    {
        // mount the resource on top of itself
        auto resPath = fs.getRealPath(path);
        fs.mount(std::make_shared<RFDMount>(resPath), path + "/");

        // update tree
        buildFileList(path, item);
    }
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
            type = identifyFile(itemPath.generic_string(), file.size);
            fileTree->SetItemText(newItem, 2, std::to_string(file.size));
        }

        // type column
        // display ext if not known
        if(type == FileType::Unknown)
            fileTree->SetItemText(newItem, 1, itemPath.extension().c_str() + 1);
        else
            fileTree->SetItemText(newItem, 1, getFileTypeLabel(type));

        // store path/type as item data
        auto itemData = new FileItemData;
        itemData->path = itemPath.generic_string();
        itemData->type = type;
        fileTree->SetItemData(newItem, itemData);

        // recurse
        // treat resource files as directories (in case they're mounted)
        if(file.isDir || type == FileType::LLResource)
            buildFileList(itemPath, newItem);
    }
}

MainFrame::FileType MainFrame::identifyFile(std::string path, uint32_t size)
{
    // get extension
    auto ext = std::filesystem::path(path).extension().string();
    // lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if(ext == ".txt" || ext == ".ini" || ext == ".str"/*legoland*/)
        return FileType::Text;
    if(ext == ".bmp" || ext == ".png" || ext == ".but"/*loco*/)
        return FileType::Image;

    if(ext == ".3d")
        return FileType::LL3D;
    // these seem to be the same format, but with different uses
    if(ext == ".csp" || ext == ".ilf" || ext == ".tsf")
        return FileType::LLCompSprite;
    if(ext == ".res") // TODO: chess has a similar format?
        return FileType::LLResource;
    if(ext == ".lls")
        return FileType::LLSprite;

    if(ext == ".crd")
    {
        // the smaller files are text (lists of cards for the easter egg characters)
        if(size < 200)
            return FileType::Text;
        else
            return FileType::LocoCard;
    }

    // loco .dat files are text
    // we don't have any game detection yet, so just make sure they're small
    if(ext == ".dat" && size < 2048)
        return FileType::Text;

    if(ext == ".rfd")
        return FileType::LocoResource;

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

        case FileType::LL3D:
            return "LL 3D";
        case FileType::LLCompSprite:
            return "LL composite sprite";
        case FileType::LLResource:
            return "LL resource";
        case FileType::LLSprite:
            return "LL sprite";

        case FileType::LocoCard:
            return "Loco card";
        case FileType::LocoResource:
            return "Loco resource";

        default:
            return "";
    }
}