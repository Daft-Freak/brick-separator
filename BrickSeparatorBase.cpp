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

	wrapperPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	fileTree = new wxTreeListCtrl( wrapperPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE );
	fileTree->SetMinSize( wxSize( 500,-1 ) );

	fileTree->AppendColumn( _("Name"), wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE|wxCOL_SORTABLE );
	fileTree->AppendColumn( _("Type"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE|wxCOL_SORTABLE );
	fileTree->AppendColumn( _("Size"), wxCOL_WIDTH_DEFAULT, wxALIGN_RIGHT, wxCOL_RESIZABLE|wxCOL_SORTABLE );

	bSizer12->Add( fileTree, 0, wxEXPAND | wxALL, 5 );

	defaultInfoPanel = new wxPanel( wrapperPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	infoLabel = new wxStaticText( defaultInfoPanel, wxID_ANY, _("Nothing to see, files are to the left."), wxDefaultPosition, wxDefaultSize, 0 );
	infoLabel->Wrap( -1 );
	bSizer2->Add( infoLabel, 0, wxALL, 5 );


	defaultInfoPanel->SetSizer( bSizer2 );
	defaultInfoPanel->Layout();
	bSizer2->Fit( defaultInfoPanel );
	bSizer12->Add( defaultInfoPanel, 1, wxEXPAND, 5 );


	wrapperPanel->SetSizer( bSizer12 );
	wrapperPanel->Layout();
	bSizer12->Fit( wrapperPanel );
	bSizer1->Add( wrapperPanel, 1, wxEXPAND | wxALL, 0 );


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

	scrollWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrollWindow->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	bitmap = new wxStaticBitmap( scrollWindow, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( bitmap, 0, wxALL, 0 );


	scrollWindow->SetSizer( bSizer13 );
	scrollWindow->Layout();
	bSizer13->Fit( scrollWindow );
	bSizer3->Add( scrollWindow, 1, wxEXPAND | wxALL, 5 );

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

CSPInfoPanelBase::CSPInfoPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );

	spritePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 10,10 ), wxTAB_TRAVERSAL );
	bSizer15->Add( spritePanel, 0, wxALL, 5 );


	bSizer15->Add( 0, 0, 1, wxEXPAND, 5 );

	spriteList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_MULTIPLE );
	spriteList->Append( _("sprite the first") );
	spriteList->Append( _("sprite the second") );
	spriteList->Append( wxEmptyString );
	bSizer15->Add( spriteList, 0, wxALL|wxEXPAND, 5 );


	bSizer3->Add( bSizer15, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	animationToggle = new wxToggleButton( this, wxID_ANY, _("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	animationToggle->SetValue( true );
	bSizer8->Add( animationToggle, 0, wxALL, 5 );

	imageInfoLabel = new wxStaticText( this, wxID_ANY, _("Some amazing sprites!"), wxDefaultPosition, wxDefaultSize, 0 );
	imageInfoLabel->Wrap( -1 );
	bSizer8->Add( imageInfoLabel, 0, wxALIGN_CENTER|wxALL, 5 );


	bSizer3->Add( bSizer8, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
	animationTimer.SetOwner( this, animationTimer.GetId() );
	animationTimer.Start( 33 );


	// Connect Events
	spritePanel->Connect( wxEVT_PAINT, wxPaintEventHandler( CSPInfoPanelBase::onSpritePaint ), NULL, this );
	animationToggle->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( CSPInfoPanelBase::onAnimationPlayToggle ), NULL, this );
	this->Connect( animationTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler( CSPInfoPanelBase::onAnimationTimer ) );
}

CSPInfoPanelBase::~CSPInfoPanelBase()
{
}

Model3DInfoPanelBase::Model3DInfoPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxGLAttributes attribs;
	attribs.PlatformDefaults().Depth(16).EndList();
	glCanvas = new wxGLCanvas(this, attribs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);
	bSizer3->Add( glCanvas, 1, wxALL|wxEXPAND, 5 );

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

	infoLabel = new wxStaticText( this, wxID_ANY, _("An amazing model!"), wxDefaultPosition, wxDefaultSize, 0 );
	infoLabel->Wrap( -1 );
	bSizer3->Add( infoLabel, 0, wxALL, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
	animationTimer.SetOwner( this, animationTimer.GetId() );
	animationTimer.Start( 33 );


	// Connect Events
	glCanvas->Connect( wxEVT_PAINT, wxPaintEventHandler( Model3DInfoPanelBase::onGLPaint ), NULL, this );
	animationToggle->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( Model3DInfoPanelBase::onAnimationPlayToggle ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	animationSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( Model3DInfoPanelBase::onAnimationSliderScroll ), NULL, this );
	this->Connect( animationTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler( Model3DInfoPanelBase::onAnimationTimer ) );
}

Model3DInfoPanelBase::~Model3DInfoPanelBase()
{
}

CRDInfoPanelBase::CRDInfoPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	scrollWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrollWindow->SetScrollRate( 5, 5 );
	scrollWindow->SetMaxSize( wxSize( -1,200 ) );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	cardPanel = new wxPanel( scrollWindow, wxID_ANY, wxDefaultPosition, wxSize( 300,200 ), wxTAB_TRAVERSAL );
	bSizer13->Add( cardPanel, 0, wxALL, 0 );


	scrollWindow->SetSizer( bSizer13 );
	scrollWindow->Layout();
	bSizer13->Fit( scrollWindow );
	bSizer3->Add( scrollWindow, 1, wxEXPAND | wxALL, 5 );

	imageInfoLabel = new wxStaticText( this, wxID_ANY, _("An amazing postcard!"), wxDefaultPosition, wxDefaultSize, 0 );
	imageInfoLabel->Wrap( -1 );
	bSizer3->Add( imageInfoLabel, 0, wxALL, 5 );

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );

	stampBitmap = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( stampBitmap, 0, wxALL, 5 );

	markBitmap = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer18->Add( markBitmap, 0, wxALL, 5 );


	bSizer3->Add( bSizer18, 0, wxEXPAND, 0 );


	this->SetSizer( bSizer3 );
	this->Layout();

	// Connect Events
	cardPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( CRDInfoPanelBase::onCardPaint ), NULL, this );
}

CRDInfoPanelBase::~CRDInfoPanelBase()
{
}
