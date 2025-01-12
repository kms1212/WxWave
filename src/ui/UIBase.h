///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-41-g7328f920)
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
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/srchctrl.h>
#include <wx/dataview.h>
#include <wx/splitter.h>
#include "ui/WaveViewerControl.hh"
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameUI
///////////////////////////////////////////////////////////////////////////////
class MainFrameUI : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_FIRST = 6000,
			wxID_LAST,
		};

		wxMenuBar* m_menubar1;
		wxMenu* menu_file;
		wxMenu* menu_edit;
		wxMenu* menu_search;
		wxMenu* menu_view;
		wxMenu* menu_time;
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* tbitem_new_file;
		wxToolBarToolBase* tbitem_open_file;
		wxToolBarToolBase* tbitem_save_file;
		wxToolBarToolBase* tbitem_save_file_as;
		wxToolBarToolBase* tbitem_first;
		wxToolBarToolBase* tbitem_backward;
		wxToolBarToolBase* tbitem_forward;
		wxToolBarToolBase* tbitem_last;
		wxToolBarToolBase* tbitem_zoom_in;
		wxToolBarToolBase* tbitem_zoom_out;
		wxToolBarToolBase* tbitem_zoom_100;
		wxToolBarToolBase* tbitem_zoom_fit;
		wxToolBarToolBase* m_tool13;
		wxToolBarToolBase* m_tool14;
		wxToolBarToolBase* m_tool15;
		wxToolBarToolBase* m_tool16;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel3;
		wxListCtrl* property_list;
		wxPanel* m_panel4;
		wxSearchCtrl* hierarchy_view_search_field;
		wxDataViewCtrl* hierarchy_view;
		wxPanel* m_panel2;
		WaveViewerControl* wave_viewer;
		wxStatusBar* m_statusBar1;

		// Virtual event handlers, override them in your derived class
		virtual void onMenuItemOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void onToolbarItemOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void onToolBarZoomInClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onToolBarZoomOutClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void onHierarchyViewSelectionChanged( wxDataViewEvent& event ) { event.Skip(); }


	public:

		MainFrameUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainFrameUI();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 240 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrameUI::m_splitter1OnIdle ), NULL, this );
		}

		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 160 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrameUI::m_splitter2OnIdle ), NULL, this );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class TracePropertyDialogUI
///////////////////////////////////////////////////////////////////////////////
class TracePropertyDialogUI : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText3;
		wxWindow* m_customControl3;
		wxRadioBox* m_radioBox1;
		wxStaticText* m_staticText1;
		wxChoice* m_choice1;
		wxStaticText* m_staticText2;
		wxChoice* m_choice2;
		wxStaticText* m_staticText10;
		wxSpinCtrl* m_spinCtrl1;
		wxCheckBox* m_checkBox1;
		wxCheckBox* m_checkBox2;
		wxCheckBox* m_checkBox3;
		wxCheckBox* m_checkBox4;
		wxCheckBox* m_checkBox5;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;

	public:

		TracePropertyDialogUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Trace Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );

		~TracePropertyDialogUI();

};

