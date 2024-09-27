#include "ui/WaveViewerControl.hh"

#include <cctype>
#include <cstdlib>

#include <vector>

#include <wx/dcbuffer.h>
#include <wx/grid.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

wxDEFINE_EVENT(WAVE_DUMMY_EVENT, wxCommandEvent);

WaveViewerControl::WaveViewerControl(wxWindow* parent, wxWindowID winid,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxPanel(parent, winid, pos, size, style, name)
    , display_start(0)
    , display_period(1000000000)
    , end_time(0)
    , wave_sel_start(-1)
    , wave_sel_end(-1)
    , wave_cursor_pos(-1)
    , root_trace_node("root")
{
}

WaveViewerControl::~WaveViewerControl() { }

IMPLEMENT_CLASS(WaveViewerControl, wxPanel)

BEGIN_EVENT_TABLE(WaveViewerControl, wxPanel)
EVT_SIZE(WaveViewerControl::onResize)
EVT_PAINT(WaveViewerControl::onPaint)
EVT_LEFT_DCLICK(WaveViewerControl::onLeftDoubleClick)
EVT_LEFT_DOWN(WaveViewerControl::onLeftDown)
EVT_LEFT_UP(WaveViewerControl::onLeftUp)
EVT_LEAVE_WINDOW(WaveViewerControl::onMouseLeave)
EVT_KEY_DOWN(WaveViewerControl::onKeyDown)
EVT_MOUSEWHEEL(WaveViewerControl::onMouseScroll)
EVT_MOTION(WaveViewerControl::onMouseMotion)

END_EVENT_TABLE()

void WaveViewerControl::calculateAreas()
{
    wxSize size = this->GetClientSize();

    this->signal_list_area = wxRect(0, time_indicator_height, signal_list_width,
        size.GetHeight() - time_indicator_height);
    this->separator_area
        = wxRect(signal_list_width, 0, separator_width, size.GetHeight());
    this->time_indicator_area = wxRect(signal_list_width + separator_width, 0,
        size.GetWidth() - signal_list_width, time_indicator_height);
    this->wave_area = wxRect(signal_list_width + separator_width,
        time_indicator_height, size.GetWidth() - signal_list_width,
        size.GetHeight() - time_indicator_height);
}

std::pair<WaveViewerNode*, int> WaveViewerControl::findNodeByMousePos(
    int mouse_y, WaveViewerNode& node, int base_y)
{
    base_y += row_gap;
    int row_height = node.IsGroup() ? 20 : node.GetRenderHeight();
    if (row_height < 20)
        row_height = 20;

    if (base_y <= mouse_y && mouse_y <= base_y + row_height + row_gap) {
        return std::make_pair<WaveViewerNode*, int>(&node, (int)base_y);
    }

    base_y += row_height + row_gap;

    if (node.IsGroup() && !node.IsFolded()) {
        for (const auto& child : node.GetChildren()) {
            auto result
                = this->findNodeByMousePos(mouse_y, *child.get(), base_y);
            if (result.first != nullptr) {
                return result;
            }

            base_y = result.second;

            if (base_y > wave_area.GetBottom()) {
                break;
            }
        }
    }

    return std::make_pair<WaveViewerNode*, int>(nullptr, (int)base_y);
}

static const wxPoint fold_handle_folded[]
    = { wxPoint(5, 5), wxPoint(0, 0), wxPoint(0, 10) };

static const wxPoint fold_handle_unfolded[]
    = { wxPoint(5, 5), wxPoint(0, 0), wxPoint(10, 0) };

int WaveViewerControl::drawNode(
    wxDC& dc, WaveViewerNode& node, int base_y, int hie_level)
{
    base_y += row_gap;
    int row_height = node.IsGroup() ? 20 : node.GetRenderHeight();
    if (row_height < 20)
        row_height = 20;

    // draw node name
    {
        wxRect name_rect(
            signal_list_area.x, base_y, signal_list_area.width, row_height);
        wxDCClipper clip(dc, name_rect.Intersect(signal_list_area));
        wxSize textSize = dc.GetTextExtent(node.GetName());

        if (&node == selection) {
            dc.SetBrush(*wxGREY_BRUSH);
            dc.DrawRectangle(name_rect);
        }

        dc.DrawText(node.GetName(),
            wxPoint(20 + hie_level * 10,
                base_y + (row_height - textSize.GetHeight()) / 2));

        if (node.IsGroup()) {
            dc.SetBrush(*wxWHITE_BRUSH);

            if (node.IsFolded()) {
                dc.DrawPolygon(3, fold_handle_folded, 4 + hie_level * 10,
                    base_y + row_height / 2 - 5);
            } else {
                dc.DrawPolygon(3, fold_handle_unfolded, 4 + hie_level * 10,
                    base_y + row_height / 2 - 2);
            }
        }
    }

    if (!node.IsGroup()) {
        // draw trace
        if (base_y + row_height >= wave_area.y) {
            wxRect trace_rect(wave_area.x, base_y, wave_area.width, row_height);
            this->drawTrace(dc, node.GetTraceHandle(), trace_rect);
        }
    }

    base_y += row_height + row_gap;

    if (base_y >= wave_area.y) {
        dc.SetPen(*wxGREY_PEN);
        dc.DrawLine(wxPoint(wave_area.GetLeft(), base_y),
            wxPoint(wave_area.GetRight(), base_y));
    }

    if (node.IsGroup() && !node.IsFolded()) {
        // draw child nodes if not folded
        for (const auto& child : node.GetChildren()) {
            base_y = this->drawNode(dc, *child.get(), base_y, hie_level + 1);

            if (base_y > wave_area.GetBottom()) {
                break;
            }
        }
    }

    return base_y;
}

static float logic_to_normalized(LogicValue logic)
{
    switch (logic.value) {
    case LogicValue::V_0:
        return 0;
    case LogicValue::V_1:
        return 1;
    case LogicValue::V_Z:
    case LogicValue::V_X:
    default:
        return 0.5;
    }
}

void WaveViewerControl::drawTrace(wxDC& dc,
    std::shared_ptr<Trace<LogicValue>> trace_handle, const wxRect& rect)
{
    wxDCClipper clip(dc, rect.Intersect(wave_area));
    std::vector<wxPoint> point_list;

    trace_handle->setCurrentTime(this->display_start);
    double xpos;
    int ypos = (1 - logic_to_normalized(trace_handle->getValue()))
        * (rect.height - 1);
    point_list.push_back(wxPoint(0, ypos));
    do {
        xpos = (double)(trace_handle->getCurrentTime() - display_start).steps
            / display_period.steps * rect.width;
        if (xpos < 0)
            continue;
        point_list.push_back(wxPoint(xpos, ypos));
        ypos = (1 - logic_to_normalized(trace_handle->getValue()))
            * (rect.height - 1);
        point_list.push_back(wxPoint(xpos, ypos));
    } while (trace_handle->seekNextTransition()
        && trace_handle->getCurrentTime() <= display_start + display_period);
    xpos = (double)(this->end_time - display_start).steps / display_period.steps
        * rect.width;
    point_list.push_back(wxPoint(xpos, ypos));

    dc.SetPen(*wxGREEN_PEN);
    dc.DrawLines(point_list.size(), point_list.data(), rect.x, rect.y);
}

void WaveViewerControl::onResize(wxSizeEvent& event) { this->calculateAreas(); }

void WaveViewerControl::onPaint(wxPaintEvent&)
{
    wxPaintDC dc(this);

    // is dark mode?
    bool isdark = wxSystemSettings::GetAppearance().IsDark();

    // fill area
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(time_indicator_area);
    dc.DrawRectangle(wave_area);

    dc.SetBrush(wxColour(isdark ? 0x222222 : 0xDDDDDD));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(signal_list_area);

    // draw time indicator / grid column
    dc.SetPen(*wxGREY_PEN);
    dc.DrawLine(time_indicator_area.GetLeftBottom(),
        time_indicator_area.GetRightBottom());
    Time marker_time = display_start;
    Time marker_gap
        = Time((int)pow(10, floor(log10(display_period.steps) - 1)));
    marker_time.steps -= marker_time.steps % marker_gap.steps;
    if (marker_time < display_start)
        marker_time.steps += marker_gap.steps;

    while (marker_time < display_start + display_period
        && marker_time <= end_time) {
        double xpos = (double)(marker_time - display_start).steps
            / display_period.steps * wave_area.GetWidth();

        {
            wxDCClipper clip(dc, time_indicator_area);
            std::string marker_text = marker_time.steps
                ? std::to_string((int)marker_time.getTime(Time::NS)) + " ns"
                : "0";
            wxSize text_size = dc.GetTextExtent(marker_text);

            dc.SetPen(*wxWHITE_PEN);
            dc.DrawLine(wxPoint(time_indicator_area.GetX() + xpos, 18),
                wxPoint(time_indicator_area.GetX() + xpos,
                    time_indicator_area.GetBottom()));
            dc.DrawText(marker_text,
                wxPoint(time_indicator_area.GetX() + xpos
                        - text_size.GetWidth() / 2,
                    time_indicator_area.GetTop() + 4));
        }

        dc.SetPen(*wxGREY_PEN);
        dc.DrawLine(
            wxPoint(time_indicator_area.GetX() + xpos, wave_area.GetTop()),
            wxPoint(time_indicator_area.GetX() + xpos, wave_area.GetBottom()));

        marker_time.steps += marker_gap.steps;
    }

    // draw wave
    this->drawNode(dc, this->root_trace_node, wave_area.y - vscroll_offset);

    // draw cursor and selection
    dc.SetPen(*wxGREY_PEN);
    if (display_start <= wave_cursor_pos
        && wave_cursor_pos <= display_start + display_period) {
        int xpos = timeToPos(wave_cursor_pos);
        dc.DrawLine(wxPoint(xpos, wave_area.GetTop()),
            wxPoint(xpos, wave_area.GetBottom()));
    }

    dc.SetPen(*wxBLUE_PEN);
    if (display_start <= wave_sel_start
        && wave_sel_start <= display_start + display_period) {
        int xpos = timeToPos(wave_sel_start);
        dc.DrawLine(wxPoint(xpos, wave_area.GetTop()),
            wxPoint(xpos, wave_area.GetBottom()));
    }
    if (display_start <= wave_sel_end
        && wave_sel_end <= display_start + display_period) {
        int xpos = timeToPos(wave_sel_end);
        dc.DrawLine(wxPoint(xpos, wave_area.GetTop()),
            wxPoint(xpos, wave_area.GetBottom()));
    }
}

void WaveViewerControl::onKeyDown(wxKeyEvent& event) { }

void WaveViewerControl::onLeftDown(wxMouseEvent& event)
{
    SetFocus();

    wxPoint mouse_pos = event.GetPosition();

    if (separator_area.Contains(mouse_pos)) {
        resizing_signal_list = true;
        this->SetCursor(wxCURSOR_SIZEWE);
    } else if (wave_area.Contains(mouse_pos)) {
        this->wave_sel_start = this->wave_sel_end
            = this->posToTime(mouse_pos.x);
    } else if (signal_list_area.Contains(mouse_pos)) {
        auto result = this->findNodeByMousePos(
            mouse_pos.y, this->root_trace_node, wave_area.y - vscroll_offset);
        this->selection = result.first;
    }

    this->Refresh(false);
}

void WaveViewerControl::onLeftDoubleClick(wxMouseEvent& event)
{
    SetFocus();

    wxPoint mouse_pos = event.GetPosition();

    if (signal_list_area.Contains(mouse_pos)) {
        auto result = this->findNodeByMousePos(
            mouse_pos.y, this->root_trace_node, wave_area.y - vscroll_offset);
        this->selection = result.first;
        if (result.first != nullptr && result.first->IsGroup()) {
            result.first->SetFoldStatus(!result.first->IsFolded());
        }
    }

    this->Refresh(false);
}

void WaveViewerControl::onLeftUp(wxMouseEvent& event)
{
    if (resizing_signal_list) {
        this->resizing_signal_list = false;
        this->SetCursor(wxCURSOR_ARROW);
    }
}

void WaveViewerControl::onMouseLeave(wxMouseEvent& event)
{
    if (resizing_signal_list) {
        this->resizing_signal_list = false;
        this->SetCursor(wxCURSOR_ARROW);
    }
}

void WaveViewerControl::onMouseMotion(wxMouseEvent& event)
{
    wxPoint mouse_pos = event.GetPosition();

    if (resizing_signal_list && mouse_pos.x > 80) {
        signal_list_width = mouse_pos.x - separator_width / 2;
        this->calculateAreas();
    } else if (wave_area.Contains(mouse_pos)) {
        if (event.Dragging()) {
            this->wave_sel_end = this->posToTime(mouse_pos.x);
        }
        this->wave_cursor_pos = this->posToTime(mouse_pos.x);
    }

    this->Refresh(false);
}

void WaveViewerControl::onMouseScroll(wxMouseEvent& event)
{
    wxPoint mouse_pos = event.GetPosition();

    if (wave_area.Contains(mouse_pos)
        && event.GetWheelAxis() == wxMouseWheelAxis::wxMOUSE_WHEEL_HORIZONTAL) {
        this->display_start.steps
            += event.GetWheelRotation() * this->display_period.steps / 400;
        if (this->display_start.steps < 0) {
            this->display_start.steps = 0;
        }
        if (this->display_start > this->end_time) {
            this->display_start = this->end_time;
        }

        this->wave_cursor_pos = this->posToTime(mouse_pos.x);
    } else if (event.GetWheelAxis()
        == wxMouseWheelAxis::wxMOUSE_WHEEL_VERTICAL) {
        this->vscroll_offset -= event.GetWheelRotation();
        if (this->vscroll_offset < 0) {
            this->vscroll_offset = 0;
        }
    }

    this->Refresh(false);
}

Time WaveViewerControl::posToTime(int xpos) const
{
    return Time(display_start.steps
        + (double)(xpos - wave_area.x) / wave_area.GetWidth()
            * display_period.steps);
}

int WaveViewerControl::timeToPos(Time time) const
{
    return wave_area.GetX()
        + (double)(wave_cursor_pos - display_start).steps / display_period.steps
        * wave_area.GetWidth();
}

WaveViewerNode& WaveViewerControl::GetRootTraceNode()
{
    return this->root_trace_node;
}

void WaveViewerControl::SetTraceEndTime(Time time) { this->end_time = time; }

Time WaveViewerControl::GetDisplayStart() const { return this->display_start; }

void WaveViewerControl::SetDisplayStart(Time time)
{
    this->display_start = time;
    this->Refresh(false);
}

Time WaveViewerControl::GetDisplayPeriod() const
{
    return this->display_period;
}

void WaveViewerControl::SetDisplayPeriod(Time time)
{
    this->display_period = time;
    this->Refresh(false);
}

WaveViewerNode* WaveViewerControl::GetSelection() const
{
    return this->selection;
}

void WaveViewerControl::Unselect() { this->selection = nullptr; }

bool WaveViewerControl::GetWaveSelection(Time* from, Time* to) const
{
    if (wave_sel_start.steps < 0) {
        return false;
    }

    *from = MIN(this->wave_sel_start, this->wave_sel_end);
    *to = MAX(this->wave_sel_start, this->wave_sel_end);
    return true;
}

bool WaveViewerControl::GetWaveCursor(Time* pos) const
{
    if (wave_cursor_pos.steps < 0) {
        return false;
    }

    *pos = this->wave_cursor_pos;
    return true;
}
