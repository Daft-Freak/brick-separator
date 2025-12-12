#pragma once

#include "BrickSeparatorBase.h"

class TextInfoPanel final : public TextInfoPanelBase
{
public:
    using TextInfoPanelBase::TextInfoPanelBase;

    void setText(wxString text) {textCtrl->SetValue(text);}
};