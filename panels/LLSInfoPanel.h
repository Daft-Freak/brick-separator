#pragma once

#include "BrickSeparatorBase.h"

#include "formats/LLSFile.h"

class LLSInfoPanel final : public LLSInfoPanelBase
{
public:
    using LLSInfoPanelBase::LLSInfoPanelBase;

    bool loadSprite(const uint8_t *data, size_t size);

private:
    void onSpritePaint(wxPaintEvent &event) override;
    void onAnimationPlayToggle(wxCommandEvent &event) override;
    void onAnimationSliderScroll(wxScrollEvent &event) override;
    void onAnimationTimer(wxTimerEvent &event) override;

    LLSFile spriteFile;
    unsigned frame = 0;
};
