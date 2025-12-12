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
