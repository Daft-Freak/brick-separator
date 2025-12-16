///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treelist.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include <wx/timer.h>
#include <wx/listbox.h>
#include <wx/glcanvas.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame
{
	private:

	protected:
		wxMenuBar* mainMenuBar;
		wxMenu* fileMenu;
		wxPanel* wrapperPanel;
		wxTreeListCtrl* fileTree;
		wxPanel* defaultInfoPanel;
		wxStaticText* infoLabel;

		// Virtual event handlers, override them in your derived class
		virtual void onOpenFolder( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFileActivated( wxTreeListEvent& event ) { event.Skip(); }
		virtual void onFileSelectionChanged( wxTreeListEvent& event ) { event.Skip(); }


	public:

		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Brick Separator"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainFrameBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class TextInfoPanelBase
///////////////////////////////////////////////////////////////////////////////
class TextInfoPanelBase : public wxPanel
{
	private:

	protected:
		wxTextCtrl* textCtrl;

	public:

		TextInfoPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~TextInfoPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ImageInfoPanelBase
///////////////////////////////////////////////////////////////////////////////
class ImageInfoPanelBase : public wxPanel
{
	private:

	protected:
		wxStaticBitmap* bitmap;
		wxStaticText* imageInfoLabel;

	public:

		ImageInfoPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~ImageInfoPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class LLSInfoPanelBase
///////////////////////////////////////////////////////////////////////////////
class LLSInfoPanelBase : public wxPanel
{
	private:

	protected:
		wxPanel* spritePanel;
		wxToggleButton* animationToggle;
		wxSlider* animationSlider;
		wxStaticText* frameNumLabel;
		wxStaticText* imageInfoLabel;
		wxTimer animationTimer;

		// Virtual event handlers, override them in your derived class
		virtual void onSpritePaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void onAnimationPlayToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAnimationSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void onAnimationTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		LLSInfoPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~LLSInfoPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CSPInfoPanelBase
///////////////////////////////////////////////////////////////////////////////
class CSPInfoPanelBase : public wxPanel
{
	private:

	protected:
		wxPanel* spritePanel;
		wxListBox* spriteList;
		wxToggleButton* animationToggle;
		wxStaticText* imageInfoLabel;
		wxTimer animationTimer;

		// Virtual event handlers, override them in your derived class
		virtual void onSpritePaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void onAnimationPlayToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAnimationTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		CSPInfoPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~CSPInfoPanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class Model3DInfoPanelBase
///////////////////////////////////////////////////////////////////////////////
class Model3DInfoPanelBase : public wxPanel
{
	private:

	protected:
		wxGLCanvas* glCanvas;
		wxToggleButton* animationToggle;
		wxSlider* animationSlider;
		wxStaticText* frameNumLabel;
		wxStaticText* infoLabel;
		wxTimer animationTimer;

		// Virtual event handlers, override them in your derived class
		virtual void onGLPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void onAnimationPlayToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void onAnimationSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void onAnimationTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		Model3DInfoPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~Model3DInfoPanelBase();

};

