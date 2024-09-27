#ifndef __WAVEVIEWERCONTROL_HH__
#define __WAVEVIEWERCONTROL_HH__

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>
#include <utility>

#include "trace.hh"

wxDECLARE_EVENT(WAVE_SCROLL_EVENT, wxCommandEvent);
wxDECLARE_EVENT(WAVE_CURSOR_EVENT, wxCommandEvent);

#define EVT_WAVE_SCROLL(winid, func)                                           \
    wx__DECLARE_EVT1(WAVE_SCROLL_EVENT, winid, wxCommandEventHandler(func))
#define EVT_WAVE_CURSOR(winid, func)                                           \
    wx__DECLARE_EVT1(WAVE_CURSOR_EVENT, winid, wxCommandEventHandler(func))

class WaveViewerNode {
private:
    bool is_group;
    bool is_folded;
    WaveViewerNode* parent;
    std::vector<std::shared_ptr<WaveViewerNode>> children;
    std::string name;
    std::string identifier;
    int render_height;
    std::shared_ptr<Trace<LogicValue>> trace_handle;

public:
    WaveViewerNode(const std::string& name, bool is_folded = false)
    {
        this->is_group = true;
        this->name = name;
        this->is_folded = is_folded;
    }
    WaveViewerNode(const std::string& name, const std::string& identifier,
        int render_height, std::shared_ptr<Trace<LogicValue>> trace_handle)
    {
        this->name = name;
        this->identifier = identifier;
        this->render_height = render_height;
        this->trace_handle = trace_handle;
    }
    ~WaveViewerNode() = default;

    const std::string& GetName() const { return this->name; }
    void SetParent(WaveViewerNode* parent) { this->parent = parent; }
    WaveViewerNode* GetParent() const { return this->parent; }

    bool IsGroup() const { return this->is_group; }
    void AddChild(std::shared_ptr<WaveViewerNode> child)
    {
        child->SetParent(this);
        this->children.push_back(child);
    }
    const std::vector<std::shared_ptr<WaveViewerNode>>& GetChildren() const
    {
        return this->children;
    }
    void ClearChildren() { this->children.clear(); }
    bool IsFolded() const { return this->is_folded; }
    void SetFoldStatus(bool folded) { this->is_folded = folded; }

    int GetRenderHeight() const { return this->render_height; }
    const std::string& GetIdentifier() const { return this->identifier; }
    std::shared_ptr<Trace<LogicValue>> GetTraceHandle()
    {
        return this->trace_handle;
    }
};

class WaveViewerControl : public wxPanel {
    DECLARE_CLASS(WaveViewerControl);
    DECLARE_EVENT_TABLE();

private:
    bool resizing_signal_list = false;

    int row_gap = 2;
    int time_indicator_height = 26;
    int separator_width = 8;
    int signal_list_width = 160;

    int vscroll_offset = 0;

    Time display_start;
    Time display_period;

    Time end_time;

    Time wave_sel_start;
    Time wave_sel_end;

    Time wave_cursor_pos;

    wxRect time_indicator_area;
    wxRect signal_list_area;
    wxRect wave_area;
    wxRect separator_area;

    WaveViewerNode root_trace_node;

    WaveViewerNode* selection;

    void calculateAreas();

    std::pair<WaveViewerNode*, int> findNodeByMousePos(
        int mouse_y, WaveViewerNode& node, int base_y);

    int drawNode(wxDC& dc, WaveViewerNode& node, int base_y, int hie_level = 0);
    void drawTrace(wxDC& dc, std::shared_ptr<Trace<LogicValue>> trace_handle,
        const wxRect& rect);

    void onResize(wxSizeEvent& event);
    void onPaint(wxPaintEvent& event);
    void onKeyDown(wxKeyEvent& event);
    void onLeftDoubleClick(wxMouseEvent& event);
    void onLeftDown(wxMouseEvent& event);
    void onLeftUp(wxMouseEvent& event);
    void onMouseLeave(wxMouseEvent& event);
    void onMouseMotion(wxMouseEvent& event);
    void onMouseScroll(wxMouseEvent& event);

    Time posToTime(int xpos) const;
    int timeToPos(Time time) const;

public:
    WaveViewerControl(wxWindow* parent, wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style
        = wxTAB_TRAVERSAL | wxNO_BORDER | wxVSCROLL | wxALWAYS_SHOW_SB,
        const wxString& name = wxASCII_STR(wxPanelNameStr));
    ~WaveViewerControl();

    WaveViewerNode& GetRootTraceNode();

    void SetTraceEndTime(Time time);

    Time GetDisplayStart() const;
    void SetDisplayStart(Time time);

    Time GetDisplayPeriod() const;
    void SetDisplayPeriod(Time time);

    WaveViewerNode* GetSelection() const;
    void Unselect();

    bool GetWaveSelection(Time* from, Time* to) const;

    bool GetWaveCursor(Time* pos) const;

    bool AcceptsFocus() const { return true; }
};

#endif // __HEXVIEWERCONTROL_HH__
