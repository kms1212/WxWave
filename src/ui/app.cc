#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/app.h>
#include <wx/cmdline.h>

#include <libxml/parser.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "app.hh"
#include "ui/MainFrame.hh"

static const wxCmdLineEntryDesc cmd_line_desc[]
    = { { wxCMD_LINE_SWITCH, _("h"), _("help"), _("show this help message"),
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
          { wxCMD_LINE_PARAM, NULL, NULL, "file to open", wxCMD_LINE_VAL_STRING,
              wxCMD_LINE_PARAM_MULTIPLE },
          { wxCMD_LINE_NONE } };

bool WxWaveApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MainFrame* main_frame;
    for (const wxString& file : this->files) {
        main_frame = new MainFrame(NULL);
        main_frame->LoadFile(file.ToStdString());
        main_frame->Show(true);
    }

    if (this->files.size() == 0) {
        main_frame = new MainFrame(NULL);
        main_frame->Show(true);
    }

    this->SetTopWindow(main_frame);

    return true;
}

void WxWaveApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(cmd_line_desc);
    parser.SetSwitchChars(wxT("-"));
}

bool WxWaveApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    for (int i = 0; i < parser.GetParamCount(); i++) {
        this->files.Add(parser.GetParam(i));
    }

    return true;
}

IMPLEMENT_APP(WxWaveApp);
IMPLEMENT_CLASS(WxWaveApp, wxApp);
IMPLEMENT_WX_THEME_SUPPORT;
