#include "ui/WaveViewerControl.hh"

#include <cctype>
#include <cstdlib>

#include <vector>

#include <wx/dcbuffer.h>
#include <wx/grid.h>

wxDEFINE_EVENT(WAVE_DUMMY_EVENT, wxCommandEvent);

WaveViewerControl::WaveViewerControl(wxWindow* parent, wxWindowID winid,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxScrolledCanvas(parent, winid, pos, size, style, name)
    , display_start(0)
    , display_period(1000000000)
    , start_time(0)
    , end_time(100000000000)
    , cursor_start(-1)
    , cursor_end(-1)
{
}

WaveViewerControl::~WaveViewerControl() { }

IMPLEMENT_CLASS(WaveViewerControl, wxScrolled)

BEGIN_EVENT_TABLE(WaveViewerControl, wxScrolled)
EVT_SIZE(WaveViewerControl::onResize)
EVT_PAINT(WaveViewerControl::onPaint)
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

    this->signal_list_area = wxRect(0, 0, signal_list_width, size.GetHeight());
    this->separator_area
        = wxRect(signal_list_width, 0, separator_width, size.GetHeight());
    this->time_indicator_area = wxRect(signal_list_width + separator_width, 0,
        size.GetWidth() - signal_list_width, time_indicator_height);
    this->wave_area = wxRect(signal_list_width + separator_width,
        time_indicator_height, size.GetWidth() - signal_list_width,
        size.GetHeight() - time_indicator_height);
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
    Time marker_gap(100, Time::NS);
    marker_time.steps -= marker_time.steps % marker_gap.steps;
    if (marker_time < display_start)
        marker_time.steps += marker_gap.steps;

    while (marker_time < display_start + display_period) {
        double xpos = (double)(marker_time - display_start).steps
            / display_period.steps * wave_area.GetWidth();

        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine(wxPoint(time_indicator_area.GetX() + xpos, 18),
            wxPoint(time_indicator_area.GetX() + xpos,
                time_indicator_area.GetBottom()));
        dc.DrawText(std::to_string((int)marker_time.getTime(Time::NS)) + " ns",
            wxPoint(time_indicator_area.GetX() + xpos,
                time_indicator_area.GetTop() + 4));

        dc.SetPen(*wxGREY_PEN);
        dc.DrawLine(
            wxPoint(time_indicator_area.GetX() + xpos, wave_area.GetTop()),
            wxPoint(time_indicator_area.GetX() + xpos, wave_area.GetBottom()));

        marker_time.steps += marker_gap.steps;
    }

    // draw wave
    int row = 0;

    int base_y = 0;
    double step_width
        = (double)wave_area.GetWidth() / this->display_period.steps;
    double transition_width = step_width / 8;
    if (transition_width > 4)
        transition_width = 4;

    for (auto& trace : this->traces) {
        base_y += row_gap;
        int wave_height = trace.height;
        if (wave_height < 20)
            wave_height = 20;

        dc.DrawText(trace.name, wxPoint(4, base_y + wave_area.GetY()));

        std::vector<wxPoint> point_list;

        trace.trace_handle->setCurrentTime(this->display_start);

        int ypos = base_y
            + (trace.trace_handle->getValue() == LogicValue::V_1 ? 0
                                                                 : wave_height);

        point_list.push_back(wxPoint(0, ypos));
        do {
            double xpos
                = (double)(trace.trace_handle->getCurrentTime() - display_start)
                      .steps
                / display_period.steps * wave_area.GetWidth();
            if (xpos < 0)
                continue;

            point_list.push_back(wxPoint(xpos - transition_width, ypos));

            ypos = base_y
                + (trace.trace_handle->getValue() == LogicValue::V_1
                        ? 0
                        : wave_height);

            point_list.push_back(wxPoint(xpos + transition_width, ypos));
        } while (trace.trace_handle->seekNextTransition()
            && trace.trace_handle->getCurrentTime()
                <= display_start + display_period);
        point_list.push_back(wxPoint(wave_area.GetWidth(), ypos));

        dc.SetPen(*wxGREEN_PEN);
        dc.DrawLines(point_list.size(), point_list.data(), wave_area.GetX(),
            wave_area.GetY());

        base_y += wave_height;
        base_y += row_gap;
        dc.SetPen(*wxGREY_PEN);
        dc.DrawLine(wave_area.GetPosition() + wxPoint(0, base_y),
            wave_area.GetPosition() + wxPoint(wave_area.GetWidth(), base_y));
        row++;
    }

    // draw cursor
    dc.SetPen(*wxBLUE_PEN);
    if (display_start <= cursor_start
        && cursor_start <= display_start + display_period) {
        double xpos = (double)(cursor_start - display_start).steps
            / display_period.steps * wave_area.GetWidth();
        dc.DrawLine(wxPoint(wave_area.GetX() + xpos, wave_area.GetTop()),
            wxPoint(wave_area.GetX() + xpos, wave_area.GetBottom()));
    }
    if (display_start <= cursor_end
        && cursor_end <= display_start + display_period) {
        double xpos = (double)(cursor_end - display_start).steps
            / display_period.steps * wave_area.GetWidth();
        dc.DrawLine(wxPoint(wave_area.GetX() + xpos, wave_area.GetTop()),
            wxPoint(wave_area.GetX() + xpos, wave_area.GetBottom()));
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
        this->cursor_start.steps = this->cursor_end.steps = display_start.steps
            + (double)(mouse_pos.x - wave_area.x) / wave_area.GetWidth()
                * display_period.steps;
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
        this->cursor_end.steps = display_start.steps
            + (double)(mouse_pos.x - wave_area.x) / wave_area.GetWidth()
                * display_period.steps;
    }

    // if (wave_area.Contains(mouse_pos)) {
    //     this->cursor_pos_end = mouse_pos.x - wave_area.GetX();
    // } else {
    //     this->cursor_pos_end = -1;
    // }

    // if (this->cursor_pos_start > this->cursor_pos_end) {
    //     int temp = this->cursor_pos_end;
    //     this->cursor_pos_end = this->cursor_pos_start;
    //     this->cursor_pos_start = temp;
    // }

    this->Refresh(false);
}

void WaveViewerControl::onMouseScroll(wxMouseEvent& event)
{
    wxPoint mouse_pos = event.GetPosition();

    if (wave_area.Contains(mouse_pos)
        && event.GetWheelAxis() == wxMouseWheelAxis::wxMOUSE_WHEEL_HORIZONTAL) {
        int64_t wheel_rot
            = event.GetWheelRotation() * this->display_period.steps / 400;
        if ((this->display_start.steps + wheel_rot >= 0)
            && ((this->display_start + this->display_period).steps + wheel_rot
                < this->end_time.steps)) {
            this->display_start.steps += wheel_rot;
        }

        this->Refresh(false);
    }
}

void WaveViewerControl::AddTrace(const std::string& name,
    const std::string& identifier, int height, Trace<LogicValue>* trace_handle)
{
    WaveViewerTrace trace { name, identifier, height, trace_handle };
    this->traces.push_back(trace);

    this->Refresh(false);
}

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

int WaveViewerControl::GetSelectionIndex() const
{
    if (this->selection >= this->traces.size())
        return -1;
    return this->selection;
}

bool WaveViewerControl::GetCursor(Time* start, Time* end) const
{
    if (cursor_start.steps < 0) {
        return false;
    }

    *start = this->cursor_start;
    *end = this->cursor_end;
    return true;
}
