#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/rawbmp.h>

#include "LLSInfoPanel.h"

LLSInfoPanel::LLSInfoPanel(wxWindow *parent) : LLSInfoPanelBase(parent)
{
    spritePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

bool LLSInfoPanel::loadSprite(const uint8_t *data, size_t size)
{
    if(!data || !spriteFile.load(data, size))
    {
        imageInfoLabel->SetLabel("Failed to load");
        return false;
    }

    // init info
    imageInfoLabel->SetLabel(wxString::Format("%ux%u, %i bits-per-pixel, %u frames, flags %08X",
        spriteFile.getWidth(),
        spriteFile.getHeight(),
        spriteFile.getBitsPerPixel(),
        spriteFile.getNumFrames(),
        spriteFile.getFlags()
    ));

    spritePanel->SetSize(spriteFile.getWidth(), spriteFile.getHeight());
    spritePanel->SetMinSize(spritePanel->GetSize());

    frameNumLabel->SetLabel(wxString::Format("0/%u", spriteFile.getNumFrames()));

    if(spriteFile.getNumFrames() > 1)
    {
        // animated
        animationSlider->SetMax(spriteFile.getNumFrames() - 1);
    }
    else
    {
        // not animated
        animationToggle->Disable();
        animationTimer.Stop();
    }

    Layout();

    return true;
}

void LLSInfoPanel::onSpritePaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(spritePanel);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

    if(!gc)
        return;

    auto size = spritePanel->GetSize();

    // draw a tiled background
    gc->SetBrush(*wxLIGHT_GREY_BRUSH);
    gc->DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    gc->SetBrush(*wxMEDIUM_GREY_BRUSH);

    const int gridSize = 16;
    for(int y = 0; y < size.GetHeight(); y += gridSize)
    {
        int xOff = ((y / gridSize) & 1) * gridSize;
        for(int x = xOff; x < size.GetWidth(); x += gridSize * 2)
            gc->DrawRectangle(x, y, gridSize, gridSize);
    }

    // decode frame
    auto buf = new uint8_t[size.GetWidth() * size.GetHeight() * 4];
    spriteFile.decodeFrame(frame, buf);

    // convert to image (wx wants separate alpha)
    wxImage image(size);
    image.InitAlpha();
    wxImagePixelData pixelData(image);
    auto iterator = pixelData.GetPixels();

    for(int i = 0; i < size.GetWidth() * size.GetHeight(); i++)
    {
        iterator.Red()   = buf[i * 4 + 0];
        iterator.Green() = buf[i * 4 + 1];
        iterator.Blue()  = buf[i * 4 + 2];
        iterator.Alpha() = buf[i * 4 + 3];
        iterator++;
    }

    gc->DrawBitmap(gc->CreateBitmapFromImage(image), 0, 0, size.GetWidth(), size.GetHeight());

    delete[] buf;

    delete gc;
}

void LLSInfoPanel::onAnimationPlayToggle(wxCommandEvent &event)
{
    if(animationToggle->GetValue())
    {
        // start timer and disable slider
        animationToggle->SetLabel("Pause");
        animationTimer.Start();
        animationSlider->Disable();
    }
    else
    {
        // stop timer and enable slider
        animationToggle->SetLabel("Play");
        animationTimer.Stop();
        animationSlider->Enable();
    }
}

void LLSInfoPanel::onAnimationSliderScroll(wxScrollEvent &event)
{
    frame = animationSlider->GetValue();
    frameNumLabel->SetLabel(wxString::Format("%u/%u", frame, spriteFile.getNumFrames()));
    // redraw
    spritePanel->Refresh();
}

void LLSInfoPanel::onAnimationTimer(wxTimerEvent &event)
{
    // advance animation and wrap
    frame++;

    if(frame >= spriteFile.getNumFrames())
        frame = 0;

    // update status and redraw
    frameNumLabel->SetLabel(wxString::Format("%u/%u", frame, spriteFile.getNumFrames()));
    animationSlider->SetValue(frame);

    spritePanel->Refresh();
}