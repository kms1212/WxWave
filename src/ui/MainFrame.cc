#include "ui/MainFrame.hh"

MainFrame::MainFrame(wxWindow* parent, wxWindowID id)
    : MainFrameUI(parent, id)
    , ghw("wave.ghw")
{
    wxDataViewColumn* col;

    // hierarchy view configuration
    col = new wxDataViewColumn("Identifier", new wxDataViewTextRenderer(), 0,
        FromDIP(120), wxALIGN_LEFT);
    this->hierarchy_view->AppendColumn(col);

    col = new wxDataViewColumn("Type", new wxDataViewTextRenderer(), 1,
        wxSIZE_AUTO_WIDTH, wxALIGN_LEFT);
    this->hierarchy_view->AppendColumn(col);

    this->hie_model = new HierarchyViewModel(this->ghw.getTop());

    this->hierarchy_view->AssociateModel(this->hie_model);

    // property list configuration
    wxListItem col0;
    col0.SetId(0);
    col0.SetText(_("Property"));
    col0.SetWidth(FromDIP(80));
    this->property_list->InsertColumn(0, col0);

    wxListItem col1;
    col1.SetId(1);
    col1.SetText(_("Value"));
    col1.SetWidth(FromDIP(160));
    this->property_list->InsertColumn(1, col1);

    // wave viewer configuration
}

MainFrame::~MainFrame() { }

void MainFrame::onHierarchyViewSelectionChanged(wxDataViewEvent& event)
{
    this->property_list->DeleteAllItems();

    if (this->hierarchy_view->HasSelection()) {
        HierarchyViewModelNode* node
            = (HierarchyViewModelNode*)this->hierarchy_view->GetCurrentItem()
                  .GetID();

        long idx = this->property_list->InsertItem(0, "Name");
        this->property_list->SetItem(idx, 1, node->GetData().getName());

        idx = this->property_list->InsertItem(idx + 1, "Type");
        this->property_list->SetItem(idx, 1, node->GetData().getSubtype());

        unsigned int sig_idx_start, sig_idx_end;
        if (node->GetData().getSignalIndexRange(&sig_idx_start, &sig_idx_end)) {
            for (int i = sig_idx_start; i <= sig_idx_end; i++) {
                this->wave_viewer->AddTrace(node->GetData().getName(),
                    node->GetData().getName(), 20,
                    new GhwTraceHandle(this->ghw, i));
            }
        }
    }
}

void MainFrame::onToolBarZoomInClicked(wxCommandEvent& event)
{
    Time current_display_period = this->wave_viewer->GetDisplayPeriod();
    current_display_period.steps -= current_display_period.steps / 10;
    this->wave_viewer->SetDisplayPeriod(current_display_period);
}

void MainFrame::onToolBarZoomOutClicked(wxCommandEvent& event)
{
    Time current_display_period = this->wave_viewer->GetDisplayPeriod();
    current_display_period.steps += current_display_period.steps / 10;
    this->wave_viewer->SetDisplayPeriod(current_display_period);
}

IMPLEMENT_CLASS(MainFrame, wxFrame)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
END_EVENT_TABLE()
