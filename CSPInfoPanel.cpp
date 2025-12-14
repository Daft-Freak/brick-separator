#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/log.h>
#include <wx/rawbmp.h>

#include "CSPInfoPanel.h"

#include "FileSystem.h"

CSPInfoPanel::CSPInfoPanel(wxWindow *parent, FileSystem &fs) : CSPInfoPanelBase(parent), fs(fs)
{
}

bool CSPInfoPanel::loadFile(const uint8_t *data, size_t size)
{
    if(!data || !compositeFile.load(data, size))
    {
        imageInfoLabel->SetLabel("Failed to load");
        return false;
    }

    // init info
    imageInfoLabel->SetLabel(wxString::Format("[%u sprites]", compositeFile.getNumSprites()));

    // load sprites and setup list
    spriteList->Clear();
    spriteFiles.clear();
    spriteFrames.clear();

    spriteFiles.reserve(compositeFile.getNumSprites());

    minX = minY = maxX = maxY = 0;

    for(unsigned i = 0; i < compositeFile.getNumSprites(); i++)
    {
        auto &spriteInfo = compositeFile.getSprite(i);

        loadSprite(spriteInfo.name);

        // need to divide by 2 or things don't line up
        int x = spriteInfo.x / 2;
        int y = spriteInfo.y / 2;
        int w = spriteFiles[i].getWidth();
        int h = spriteFiles[i].getHeight();

        // calculate bounds
        if(x < minX)
            minX = x;
        if(x + w > maxX)
            maxX = x + w;

        if(y < minY)
            minY = y;
        if(y + h > maxY)
            maxY = y + h;

        // list item
        auto newItem = spriteList->Append(wxString::Format("%s at %i,%i", spriteInfo.name, spriteInfo.x, spriteInfo.y));
        spriteList->SetSelection(newItem);
    }

    spritePanel->SetSize(maxX - minX, maxY - minY);
    spritePanel->SetMinSize(spritePanel->GetSize());

    Layout();

    return true;
}

void CSPInfoPanel::onSpritePaint(wxPaintEvent &event)
{
    wxPaintDC dc(spritePanel);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(!gc)
        return;

    auto panelSize = spritePanel->GetSize();

    // draw a tiled background
    gc->SetBrush(*wxLIGHT_GREY_BRUSH);
    gc->DrawRectangle(0, 0, panelSize.GetWidth(), panelSize.GetHeight());

    gc->SetBrush(*wxMEDIUM_GREY_BRUSH);

    const int gridSize = 16;
    for(int y = 0; y < panelSize.GetHeight(); y += gridSize)
    {
        int xOff = ((y / gridSize) & 1) * gridSize;
        for(int x = xOff; x < panelSize.GetWidth(); x += gridSize * 2)
            gc->DrawRectangle(x, y, gridSize, gridSize);
    }

    // decode frame
    for(unsigned i = 0; i < compositeFile.getNumSprites(); i++)
    {
        auto spriteW = spriteFiles[i].getWidth();
        auto spriteH = spriteFiles[i].getHeight();

        // skip failed sprites
        if(!spriteW)
            continue;

        // only draw sprite if selected in list
        if(!spriteList->IsSelected(i))
            continue;

        auto buf = new uint8_t[spriteW * spriteH * 4];
        spriteFiles[i].decodeFrame(spriteFrames[i], buf);

        // convert to image (wx wants separate alpha)
        wxImage image(spriteW, spriteH);
        image.InitAlpha();
        wxImagePixelData pixelData(image);
        auto iterator = pixelData.GetPixels();

        for(int i = 0; i < spriteW * spriteH; i++)
        {
            iterator.Red()   = buf[i * 4 + 0];
            iterator.Green() = buf[i * 4 + 1];
            iterator.Blue()  = buf[i * 4 + 2];
            iterator.Alpha() = buf[i * 4 + 3];
            iterator++;
        }

        auto &spriteInfo = compositeFile.getSprite(i);
        int x = spriteInfo.x / 2 - minX;
        int y = spriteInfo.y / 2 - minY;

        gc->DrawBitmap(gc->CreateBitmapFromImage(image), x, y, spriteW, spriteH);

        delete[] buf;
    }

    delete gc;
}

void CSPInfoPanel::onAnimationPlayToggle(wxCommandEvent &event)
{
    if(animationToggle->GetValue())
    {
        // start timer and disable slider
        animationToggle->SetLabel("Pause");
        animationTimer.Start();
    }
    else
    {
        // stop timer and enable slider
        animationToggle->SetLabel("Play");
        animationTimer.Stop();
    }
}

void CSPInfoPanel::onAnimationTimer(wxTimerEvent &event)
{
    // advance animations and wrap
    for(unsigned i = 0; i < compositeFile.getNumSprites(); i++)
    {
        spriteFrames[i]++;

        if(spriteFrames[i] >= spriteFiles[i].getNumFrames())
            spriteFrames[i] = 0;
    }

    // redraw
    spritePanel->Refresh();
}

void CSPInfoPanel::loadSprite(const std::string &name)
{
    // assuming game folder is mounted at root
    // and the graphics res files have been copied to /Volumes/
    // ... and have been mounted
    const char *searchPaths[]
    {
        "",
        "Volumes/Graphics1.res/Graphics/",
        "Volumes/Graphics2.res/Graphics/small/"
    };

    spriteFiles.emplace_back();
    spriteFrames.emplace_back(0);

    // try paths until one works
    bool loaded = false;
    for(auto &searchPath : searchPaths)
    {
        auto path = std::string(searchPath).append(name);

        auto data = fs.getFileContents(path);

        if(!data)
            continue;

        if(spriteFiles.back().load(data->data(), data->size()))
        {
            loaded = true;
            break;
        }
    }

    if(!loaded)
        wxLogMessage("Failed to load %s", name);
}
