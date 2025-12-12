#include <wx/wx.h>

#include "MainFrame.h"

class BrickSeparatorApp : public wxApp
{
public:
    bool OnInit() override;
};
 
wxIMPLEMENT_APP(BrickSeparatorApp);

bool BrickSeparatorApp::OnInit()
{
    wxInitAllImageHandlers();

    auto frame = new MainFrame();
    frame->Show();
    return true;
}
