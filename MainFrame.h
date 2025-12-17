#pragma once

#include "BrickSeparatorBase.h"

#include "filesystem/FileSystem.h"

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
        LL3D,
        LLCompSprite,
        LLResource,
        LLSprite,

        // loco
        LocoResource, // RFD/RFH
    };

    class FileItemData final : public wxClientData
    {
    public:
        std::string path;
        FileType type;
    };

    // events
    void onOpenFolder(wxCommandEvent &event) override;
    void onFileSelectionChanged(wxTreeListEvent &event) override;
    void onFileActivated(wxTreeListEvent &event) override;

    void buildFileList(std::filesystem::path path, wxTreeListItem parent);

    FileType identifyFile(std::string path, uint32_t size);
    std::string getFileTypeLabel(FileType type);

    FileSystem fs;

    wxPanel *curInfoPanel;
};