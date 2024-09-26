#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/app.h>

#include <libxml/parser.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "app.hh"
#include "ui/MainFrame.hh"

bool WxWaveApp::OnInit()
{
    MainFrame* main_frame = new MainFrame(NULL);
    main_frame->Show(true);

    return true;
}

IMPLEMENT_APP(WxWaveApp);
IMPLEMENT_CLASS(WxWaveApp, wxApp);
IMPLEMENT_WX_THEME_SUPPORT;
