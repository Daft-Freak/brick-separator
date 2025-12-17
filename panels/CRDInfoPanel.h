#pragma once

#include <map>

#include "BrickSeparatorBase.h"

#include "formats/CRDFile.h"

class FileSystem;

class CRDInfoPanel final : public CRDInfoPanelBase
{
public:
    CRDInfoPanel(wxWindow *parent, FileSystem &fs);

    bool loadFile(const uint8_t *data, size_t size);

private:
    void onCardPaint(wxPaintEvent &event) override;

    wxImage loadImage(std::string_view name);

    FileSystem &fs;

    CRDFile cardFile;

    std::map<std::string, wxImage, std::less<>> imageCache;
};