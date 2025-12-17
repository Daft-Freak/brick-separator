#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/mstream.h>

#include "CRDInfoPanel.h"

#include "filesystem/FileSystem.h"

CRDInfoPanel::CRDInfoPanel(wxWindow *parent, FileSystem &fs) : CRDInfoPanelBase(parent), fs(fs)
{
    cardPanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

bool CRDInfoPanel::loadFile(const uint8_t *data, size_t size)
{
    if(!data || !cardFile.load(data, size))
    {
        imageInfoLabel->SetLabel("Failed to load");
        return false;
    }

    imageInfoLabel->SetLabel(wxString::Format("To: %s\nFrom: %s\n\n%s", cardFile.getToName(), cardFile.getFromName(), cardFile.getMessage()));

    // display stamp/mark
    auto filename = cardFile.getStampFilename();
    if(!filename.empty())
    {
        auto stampImage = loadImage(filename);
        stampBitmap->SetBitmap(wxBitmapBundle::FromImage(stampImage));
    }

    filename = cardFile.getMarkFilename();
    if(!filename.empty())
    {
        auto markImage = loadImage(filename);
        markBitmap->SetBitmap(wxBitmapBundle::FromImage(markImage));
    }

    return true;
}

void CRDInfoPanel::onCardPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(cardPanel);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(!gc)
        return;

    auto size = cardPanel->GetSize();

    uint8_t rgb[3];
    cardFile.getBackgroundColourRGB(rgb);
    auto bgBrush = gc->CreateBrush(wxBrush(wxColour(rgb[0], rgb[1], rgb[2])));

    // fill background
    gc->SetBrush(bgBrush);
    gc->DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    // draw images
    for(int i = 0; i < 128; i++)
    {
        auto &clip = cardFile.getClipart(i);

        if(clip.index == 0)
            continue;

        auto image = loadImage(cardFile.getClipartFilename(i));

        // the file does contain the image width/height
        // but that doesn't seem to be used for positioning
        int x = clip.x * 2 - image.GetWidth() / 2;
        int y = clip.y * 2 - image.GetHeight() / 2;

        gc->DrawBitmap(gc->CreateBitmapFromImage(image), x, y, image.GetWidth(), image.GetHeight());
    }
}

wxImage CRDInfoPanel::loadImage(std::string_view name)
{
    // check cache
    auto it = imageCache.find(name);

    if(it != imageCache.end())
        return it->second;

    // load it
    auto path = std::string("art-res/resource.RFD/clipart/").append(name);

    auto contents = fs.getFileContents(path);
    wxImage image;

    if(!contents)
    {
        wxLogMessage("Failed to load %s", path);
        return {};
    }

    wxMemoryInputStream stream(contents->data(), contents->size());
    image.LoadFile(stream);

    // colour key
    // technically, this should be palette index 0...
    image.SetMaskColour(255, 0, 255);

    imageCache.emplace(name, image);

    return image;
}