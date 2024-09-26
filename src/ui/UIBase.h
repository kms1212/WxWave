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

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameUI
///////////////////////////////////////////////////////////////////////////////
class MainFrameUI : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar1;
		wxMenu* menu_file;
		wxMenu* menu_edit;
		wxMenu* menu_search;
		wxMenu* menu_view;
		wxMenu* menu_time;
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_tool1;
		wxToolBarToolBase* m_tool2;
		wxToolBarToolBase* m_tool3;
		wxToolBarToolBase* m_tool4;
		wxToolBarToolBase* m_tool5;
		wxToolBarToolBase* m_tool6;
		wxToolBarToolBase* m_tool7;
		wxToolBarToolBase* m_tool8;
		wxToolBarToolBase* tbitem_zoom_in;
		wxToolBarToolBase* tbitem_zoom_out;
		wxToolBarToolBase* m_tool11;
		wxToolBarToolBase* m_tool12;
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

