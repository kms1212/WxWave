#ifndef __WAVEVIEWERCONTROL_HH__
#define __WAVEVIEWERCONTROL_HH__

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>

#include "trace.hh"

wxDECLARE_EVENT(WAVE_DUMMY_EVENT, wxCommandEvent);

#define EVT_WAVE_DUMMY_EVENT(winid, func)                                      \
    wx__DECLARE_EVT1(WAVE_DUMMY_EVENT, winid, wxCommandEventHandler(func))

struct WaveViewerTrace {
    std::string name;
    std::string identifier;
    int height;
    Trace<LogicValue>* trace_handle;
};

class WaveViewerControl : public wxScrolledCanvas {
    DECLARE_CLASS(WaveViewerControl);
    DECLARE_EVENT_TABLE();

private:
    bool resizing_signal_list = false;

    int row_gap = 6;
    int time_indicator_height = 26;
    int separator_width = 8;
    int signal_list_width = 160;

    Time display_start;
    Time display_period;

    Time start_time;
    Time end_time;

    Time cursor_start;
    Time cursor_end;

    wxRect time_indicator_area;
    wxRect signal_list_area;
    wxRect wave_area;
    wxRect separator_area;

    std::vector<WaveViewerTrace> traces;

    int selection = -1;

    void calculateAreas();

    void onResize(wxSizeEvent& event);
    void onPaint(wxPaintEvent& event);
    void onKeyDown(wxKeyEvent& event);
    void onLeftDown(wxMouseEvent& event);
    void onLeftUp(wxMouseEvent& event);
    void onMouseLeave(wxMouseEvent& event);
    void onMouseMotion(wxMouseEvent& event);
    void onMouseScroll(wxMouseEvent& event);

public:
    WaveViewerControl(wxWindow* parent, wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style
        = wxTAB_TRAVERSAL | wxNO_BORDER | wxVSCROLL | wxALWAYS_SHOW_SB,
        const wxString& name = wxASCII_STR(wxPanelNameStr));
    ~WaveViewerControl();

    void AddTrace(const std::string& name, const std::string& identifier,
        int height, Trace<LogicValue>* trace_handle);

    Time GetDisplayStart() const;
    void SetDisplayStart(Time time);

    Time GetDisplayPeriod() const;
    void SetDisplayPeriod(Time time);

    int GetSelectionIndex() const;

    bool GetCursor(Time* start, Time* end) const;

    bool AcceptsFocus() const { return true; }
};

#endif // __HEXVIEWERCONTROL_HH__
