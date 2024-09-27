#ifndef __UI_APP_HH__
#define __UI_APP_HH__

#include <wx/app.h>

class WxWaveApp : public wxApp {
    DECLARE_CLASS(WxWaveApp);

private:
    wxArrayString files;

public:
    virtual bool OnInit() override;

    virtual void OnInitCmdLine(wxCmdLineParser& parser) override;
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser) override;
};

#endif // __UI_APP_HH__
