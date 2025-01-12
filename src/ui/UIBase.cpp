///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-41-g7328f920)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "UIBase.h"

///////////////////////////////////////////////////////////////////////////

MainFrameUI::MainFrameUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	menu_file = new wxMenu();
	wxMenuItem* mitem_new;
	mitem_new = new wxMenuItem( menu_file, wxID_ANY, wxString( _("New") ) + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( mitem_new );

	wxMenuItem* mitem_new_window;
	mitem_new_window = new wxMenuItem( menu_file, wxID_ANY, wxString( _("New Window") ) + wxT('\t') + wxT("CTRL+ALT+N"), wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( mitem_new_window );

	menu_file->AppendSeparator();

	wxMenuItem* mitem_open;
	mitem_open = new wxMenuItem( menu_file, wxID_ANY, wxString( _("Open") ) + wxT('\t') + wxT("CTRL+O"), wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( mitem_open );

	wxMenuItem* mitem_save;
	mitem_save = new wxMenuItem( menu_file, wxID_ANY, wxString( _("Save") ) + wxT('\t') + wxT("CTRL+S"), wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( mitem_save );

	wxMenuItem* mitem_save_as;
	mitem_save_as = new wxMenuItem( menu_file, wxID_ANY, wxString( _("Save As...") ) + wxT('\t') + wxT("CTRL+SHIFT+S"), wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( mitem_save_as );

	m_menubar1->Append( menu_file, _("File") );

	menu_edit = new wxMenu();
	wxMenuItem* mitem_undo;
	mitem_undo = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Undo") ) + wxT('\t') + wxT("CTRL+Z"), wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_undo );

	wxMenuItem* mitem_redo;
	mitem_redo = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Redo") ) + wxT('\t') + wxT("CTRL+SHIFT+Z"), wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_redo );

	menu_edit->AppendSeparator();

	wxMenuItem* mitem_duplicate_trace;
	mitem_duplicate_trace = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Duplicate Trace") ) + wxT('\t') + wxT("CTRL+D"), wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_duplicate_trace );

	wxMenuItem* mitem_delete_trace;
	mitem_delete_trace = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Delete Trace") ) + wxT('\t') + wxT("BACKSPACE"), wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_delete_trace );

	menu_edit->AppendSeparator();

	wxMenuItem* mitem_edit_trace_properties;
	mitem_edit_trace_properties = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Edit Trace Properties") ) + wxT('\t') + wxT("CTRL+E"), wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_edit_trace_properties );

	menu_edit->AppendSeparator();

	wxMenuItem* mitem_expand;
	mitem_expand = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Expand") ) , wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_expand );

	menu_edit->AppendSeparator();

	wxMenuItem* mitem_group;
	mitem_group = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Create Group") ) + wxT('\t') + wxT("CTRL+G"), wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_group );

	wxMenuItem* mitem_fold_unfold_group;
	mitem_fold_unfold_group = new wxMenuItem( menu_edit, wxID_ANY, wxString( _("Fold/Unfold Group") ) , wxEmptyString, wxITEM_NORMAL );
	menu_edit->Append( mitem_fold_unfold_group );

	m_menubar1->Append( menu_edit, _("Edit") );

	menu_search = new wxMenu();
	m_menubar1->Append( menu_search, _("Search") );

	menu_view = new wxMenu();
	m_menubar1->Append( menu_view, _("View") );

	menu_time = new wxMenu();
	m_menubar1->Append( menu_time, _("Time") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolBar1->SetToolBitmapSize( wxSize( 32,32 ) );
	tbitem_new_file = m_toolBar1->AddTool( wxID_NEW, _("New File"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_NEW), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, _("New File"), wxEmptyString, NULL );

	tbitem_open_file = m_toolBar1->AddTool( wxID_OPEN, _("Open File"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_FILE_OPEN), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, _("Open File"), wxEmptyString, NULL );

	tbitem_save_file = m_toolBar1->AddTool( wxID_SAVE, _("Save File"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_FILE_SAVE), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, _("Save File"), wxEmptyString, NULL );

	tbitem_save_file_as = m_toolBar1->AddTool( wxID_SAVEAS, _("Save As..."), wxArtProvider::GetBitmap( wxASCII_STR(wxART_FILE_SAVE_AS), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, _("Save As..."), wxEmptyString, NULL );

	m_toolBar1->AddSeparator();

	tbitem_first = m_toolBar1->AddTool( wxID_FIRST, _("tool"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_GOTO_FIRST), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	tbitem_backward = m_toolBar1->AddTool( wxID_BACKWARD, _("tool"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_GO_BACK), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	tbitem_forward = m_toolBar1->AddTool( wxID_FORWARD, _("tool"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_GO_FORWARD), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	tbitem_last = m_toolBar1->AddTool( wxID_LAST, _("tool"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_GOTO_LAST), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->AddSeparator();

	tbitem_zoom_in = m_toolBar1->AddTool( wxID_ZOOM_IN, _("Zoom In"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_PLUS), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, _("Zoom In"), wxEmptyString, NULL );

	tbitem_zoom_out = m_toolBar1->AddTool( wxID_ANY, _("Zoom Out"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_MINUS), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, _("Zoom Out"), wxEmptyString, NULL );

	tbitem_zoom_100 = m_toolBar1->AddTool( wxID_ZOOM_100, _("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	tbitem_zoom_fit = m_toolBar1->AddTool( wxID_ZOOM_FIT, _("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->AddSeparator();

	m_tool13 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxArtProvider::GetBitmap( wxASCII_STR(wxART_FIND), wxASCII_STR(wxART_TOOLBAR) ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_tool14 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_tool15 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_tool16 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	m_toolBar1->Realize();

	bSizer13->Add( m_toolBar1, 0, wxEXPAND, 5 );

	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrameUI::m_splitter1OnIdle ), NULL, this );

	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_splitter2 = new wxSplitterWindow( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrameUI::m_splitter2OnIdle ), NULL, this );

	m_panel3 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	property_list = new wxListCtrl( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer9->Add( property_list, 1, wxALL|wxEXPAND, 5 );


	m_panel3->SetSizer( bSizer9 );
	m_panel3->Layout();
	bSizer9->Fit( m_panel3 );
	m_panel4 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	hierarchy_view_search_field = new wxSearchCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	hierarchy_view_search_field->ShowSearchButton( true );
	#endif
	hierarchy_view_search_field->ShowCancelButton( false );
	bSizer10->Add( hierarchy_view_search_field, 0, wxALL|wxEXPAND, 5 );

	hierarchy_view = new wxDataViewCtrl( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( hierarchy_view, 1, wxALL|wxEXPAND, 5 );


	m_panel4->SetSizer( bSizer10 );
	m_panel4->Layout();
	bSizer10->Fit( m_panel4 );
	m_splitter2->SplitHorizontally( m_panel3, m_panel4, 160 );
	bSizer4->Add( m_splitter2, 1, wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer4 );
	m_panel1->Layout();
	bSizer4->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	wave_viewer = new WaveViewerControl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( wave_viewer, 1, wxALL|wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer5 );
	m_panel2->Layout();
	bSizer5->Fit( m_panel2 );
	m_splitter1->SplitVertically( m_panel1, m_panel2, 240 );
	bSizer13->Add( m_splitter1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer13 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameUI::onMenuItemOpen ), this, mitem_open->GetId());
	this->Connect( tbitem_open_file->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameUI::onToolbarItemOpen ) );
	this->Connect( tbitem_zoom_in->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameUI::onToolBarZoomInClicked ) );
	this->Connect( tbitem_zoom_out->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrameUI::onToolBarZoomOutClicked ) );
	hierarchy_view->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( MainFrameUI::onHierarchyViewSelectionChanged ), NULL, this );
}

MainFrameUI::~MainFrameUI()
{
}

TracePropertyDialogUI::TracePropertyDialogUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer10->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_customControl3 = new wxWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_customControl3, 1, wxALL|wxEXPAND, 5 );


	bSizer6->Add( bSizer10, 1, wxEXPAND, 5 );

	wxString m_radioBox1Choices[] = { _("Digital"), _("Analog") };
	int m_radioBox1NChoices = sizeof( m_radioBox1Choices ) / sizeof( wxString );
	m_radioBox1 = new wxRadioBox( this, wxID_ANY, _("Visualization"), wxDefaultPosition, wxDefaultSize, m_radioBox1NChoices, m_radioBox1Choices, 1, wxRA_SPECIFY_ROWS );
	m_radioBox1->SetSelection( 0 );
	bSizer6->Add( m_radioBox1, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Value Base"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer7->Add( m_staticText1, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choice1Choices[] = { _("Hexadecimal"), _("Decimal"), _("Signed Decimal"), _("Binary"), _("Octal"), _("ASCII") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer7->Add( m_choice1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer6->Add( bSizer7, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer8->Add( m_staticText2, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choice2Choices[] = { _("Red"), _("Orange"), _("Yellow"), _("Green"), _("Blue"), _("Purple"), _("White"), _("Pink") };
	int m_choice2NChoices = sizeof( m_choice2Choices ) / sizeof( wxString );
	m_choice2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice2NChoices, m_choice2Choices, 0 );
	m_choice2->SetSelection( 3 );
	bSizer8->Add( m_choice2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer6->Add( bSizer8, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText10 = new wxStaticText( this, wxID_ANY, _("Trace Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer9->Add( m_staticText10, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_spinCtrl1 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 20, 200, 0 );
	bSizer9->Add( m_spinCtrl1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer6->Add( bSizer9, 0, wxEXPAND, 5 );

	m_checkBox1 = new wxCheckBox( this, wxID_ANY, _("RIght Justify"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkBox1, 0, wxALL|wxEXPAND, 5 );

	m_checkBox2 = new wxCheckBox( this, wxID_ANY, _("Invert"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkBox2, 0, wxALL|wxEXPAND, 5 );

	m_checkBox3 = new wxCheckBox( this, wxID_ANY, _("Reverse"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkBox3, 0, wxALL|wxEXPAND, 5 );

	m_checkBox4 = new wxCheckBox( this, wxID_ANY, _("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkBox4, 0, wxALL|wxEXPAND, 5 );

	m_checkBox5 = new wxCheckBox( this, wxID_ANY, _("Check Me!"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkBox5, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );


	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();

	bSizer11->Add( m_sdbSizer1, 3, wxEXPAND, 5 );


	bSizer6->Add( bSizer11, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );

	this->Centre( wxBOTH );
}

TracePropertyDialogUI::~TracePropertyDialogUI()
{
}
