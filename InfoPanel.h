#pragma once

#include "BrickSeparatorBase.h"

// these are the generic panels

class TextInfoPanel final : public TextInfoPanelBase
{
public:
    using TextInfoPanelBase::TextInfoPanelBase;

    void setText(wxString text) {textCtrl->SetValue(text);}
};


class ImageInfoPanel final : public ImageInfoPanelBase
{
public:
    using ImageInfoPanelBase::ImageInfoPanelBase;

    void setImage(const wxImage &image);
};