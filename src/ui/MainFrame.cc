#include "ui/MainFrame.hh"

MainFrame::MainFrame(wxWindow* parent, wxWindowID id)
    : MainFrameUI(parent, id)
{
    wxDataViewColumn* col;

    // hierarchy view configuration
    col = new wxDataViewColumn("Identifier", new wxDataViewTextRenderer(), 0,
        FromDIP(120), wxALIGN_LEFT);
    this->hierarchy_view->AppendColumn(col);

    col = new wxDataViewColumn("Type", new wxDataViewTextRenderer(), 1,
        wxSIZE_AUTO_WIDTH, wxALIGN_LEFT);
    this->hierarchy_view->AppendColumn(col);

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

MainFrame::~MainFrame() { this->UnloadFile(); }

void MainFrame::UnloadFile()
{
    this->hierarchy_view->AssociateModel(nullptr);
    this->property_list->DeleteAllItems();
    this->wave_viewer->GetRootTraceNode().ClearChildren();
    ghw.release();
    this->wave_viewer->Unselect();
}

void MainFrame::LoadFile(const std::string& path)
{
    this->UnloadFile();

    ghw.reset(new GhwFile(path));
    this->SetTitle("WxWave - " + path);
    this->hie_model.reset(new HierarchyViewModel(this->ghw->getTop()));
    this->hierarchy_view->AssociateModel(this->hie_model.get());
    this->wave_viewer->SetTraceEndTime(ghw->getEndTime());
    this->wave_viewer->GetRootTraceNode().AddChild(
        std::make_shared<WaveViewerNode>("node"));
    this->wave_viewer->Refresh(false);
    this->hierarchy_view->Select(this->hierarchy_view->GetTopItem());
    this->hierarchy_view->Expand(this->hierarchy_view->GetTopItem());
}

void MainFrame::onMenuItemOpen(wxCommandEvent& event)
{
    wxFileDialog ofd(this, _("Open wave file"), "", "",
        "GHDL Wave (*.ghw)|*.ghw|All Files (*.*)|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (ofd.ShowModal() == wxID_CANCEL) {
        return;
    }

    this->LoadFile(ofd.GetPath().ToStdString());
}

void MainFrame::onToolbarItemOpen(wxCommandEvent& event)
{
    wxFileDialog ofd(this, _("Open wave file"), "", "",
        "GHDL Wave (*.ghw)|*.ghw|All Files (*.*)|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (ofd.ShowModal() == wxID_CANCEL) {
        return;
    }

    this->LoadFile(ofd.GetPath().ToStdString());
}

void MainFrame::onHierarchyViewSelectionChanged(wxDataViewEvent& event)
{
    this->property_list->DeleteAllItems();

    if (this->hierarchy_view->HasSelection()) {
        HierarchyViewModelNode* node
            = (HierarchyViewModelNode*)this->hierarchy_view->GetCurrentItem()
                  .GetID();

        long idx = this->property_list->InsertItem(0, "Name");
        this->property_list->SetItem(idx, 1, node->GetData().getPath());

        idx = this->property_list->InsertItem(idx + 1, "Type");
        this->property_list->SetItem(idx, 1, node->GetData().getSubtype());

        unsigned int sig_idx_start, sig_idx_end;
        if (node->GetData().getSignalIndexRange(&sig_idx_start, &sig_idx_end)) {
            for (int i = sig_idx_start; i <= sig_idx_end; i++) {
                this->wave_viewer->GetRootTraceNode().AddChild(
                    std::make_shared<WaveViewerNode>(node->GetData().getName(),
                        node->GetData().getName(), 20,
                        std::make_shared<GhwTraceHandle>(*this->ghw.get(), i)));
                this->wave_viewer->Refresh(false);
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
