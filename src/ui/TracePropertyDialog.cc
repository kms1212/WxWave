#include "TracePropertyDialog.hh"

TracePropertyDialog::TracePropertyDialog(wxWindow* parent, wxWindowID id)
    : TracePropertyDialogUI(parent, id)
{
}

TracePropertyDialog::~TracePropertyDialog() { }

IMPLEMENT_CLASS(TracePropertyDialog, wxDialog)

BEGIN_EVENT_TABLE(TracePropertyDialog, wxDialog)
END_EVENT_TABLE()
