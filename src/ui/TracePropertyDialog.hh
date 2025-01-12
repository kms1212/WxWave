#ifndef __TRACEPROPERTYDIALOG__
#define __TRACEPROPERTYDIALOG__

#include "UIBase.h"

class TracePropertyDialog : public TracePropertyDialogUI {
    DECLARE_CLASS(TracePropertyDialog)
    DECLARE_EVENT_TABLE();

private:
public:
    TracePropertyDialog(wxWindow* parent, wxWindowID id = wxID_ANY);
    ~TracePropertyDialog();
};

#endif // __TRACEPROPERTYDIALOG__
