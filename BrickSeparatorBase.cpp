///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "BrickSeparatorBase.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	mainMenuBar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* openFolderItem;
	openFolderItem = new wxMenuItem( fileMenu, wxID_ANY, wxString( _("&Open Folder") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	openFolderItem->SetBitmaps( wxNullBitmap );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	openFolderItem->SetBitmap( wxNullBitmap );
	#endif
	fileMenu->Append( openFolderItem );

	mainMenuBar->Append( fileMenu, _("&File") );

	this->SetMenuBar( mainMenuBar );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	fileTree = new wxTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE );
	fileTree->SetMinSize( wxSize( 500,-1 ) );

	fileTree->AppendColumn( _("Name"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE|wxCOL_SORTABLE );
	fileTree->AppendColumn( _("Type"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE|wxCOL_SORTABLE );
	fileTree->AppendColumn( _("Size"), wxCOL_WIDTH_DEFAULT, wxALIGN_RIGHT, wxCOL_RESIZABLE|wxCOL_SORTABLE );

	bSizer1->Add( fileTree, 0, wxEXPAND | wxALL, 5 );

	defaultInfoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	infoLabel = new wxStaticText( defaultInfoPanel, wxID_ANY, _("Nothing to see, files are to the left."), wxDefaultPosition, wxDefaultSize, 0 );
	infoLabel->Wrap( -1 );
	bSizer2->Add( infoLabel, 0, wxALL, 5 );


	defaultInfoPanel->SetSizer( bSizer2 );
	defaultInfoPanel->Layout();
	bSizer2->Fit( defaultInfoPanel );
	bSizer1->Add( defaultInfoPanel, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onOpenFolder ), this, openFolderItem->GetId());
	fileTree->Connect( wxEVT_TREELIST_ITEM_ACTIVATED, wxTreeListEventHandler( MainFrameBase::onFileActivated ), NULL, this );
	fileTree->Connect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( MainFrameBase::onFileSelectionChanged ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
}

TextInfoPanelBase::TextInfoPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	textCtrl = new wxTextCtrl( this, wxID_ANY, _("this\nis\nsome\ntext"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer3->Add( textCtrl, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
}

TextInfoPanelBase::~TextInfoPanelBase()
{
}

ImageInfoPanelBase::ImageInfoPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	bitmap = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( bitmap, 0, wxALL, 5 );

	imageInfoLabel = new wxStaticText( this, wxID_ANY, _("An amazing image!"), wxDefaultPosition, wxDefaultSize, 0 );
	imageInfoLabel->Wrap( -1 );
	bSizer3->Add( imageInfoLabel, 0, wxALL, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
}

ImageInfoPanelBase::~ImageInfoPanelBase()
{
}

LLSInfoPanelBase::LLSInfoPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	spritePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 10,10 ), wxTAB_TRAVERSAL );
	bSizer3->Add( spritePanel, 0, wxALL, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	animationToggle = new wxToggleButton( this, wxID_ANY, _("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	animationToggle->SetValue( true );
	bSizer8->Add( animationToggle, 0, wxALL, 5 );

	animationSlider = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	animationSlider->Enable( false );

	bSizer8->Add( animationSlider, 1, wxALIGN_CENTER|wxALL, 5 );

	frameNumLabel = new wxStaticText( this, wxID_ANY, _("999/999"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	frameNumLabel->Wrap( -1 );
	frameNumLabel->SetMinSize( wxSize( 60,-1 ) );

	bSizer8->Add( frameNumLabel, 0, wxALIGN_CENTER|wxALL, 5 );


	bSizer3->Add( bSizer8, 0, wxEXPAND, 5 );

	imageInfoLabel = new wxStaticText( this, wxID_ANY, _("An amazing sprite!"), wxDefaultPosition, wxDefaultSize, 0 );
	imageInfoLabel->Wrap( -1 );
	bSizer3->Add( imageInfoLabel, 0, wxALL, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
	animationTimer.SetOwner( this, animationTimer.GetId() );
	animationTimer.Start( 33 );


	// Connect Events
	spritePanel->Connect( wxEVT_PAINT, wxPaintEventHandler( LLSInfoPanelBase::onSpritePaint ), NULL, this );
	animationToggle->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( LLSInfoPanelBase::onAnimationPlayToggle ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( LLSInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	this->Connect( animationTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler( LLSInfoPanelBase::onAnimationTimer ) );
}

LLSInfoPanelBase::~LLSInfoPanelBase()
{
}
