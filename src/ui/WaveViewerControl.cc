#include "ui/WaveViewerControl.hh"

#include <cctype>
#include <cstdlib>

#include <vector>

#include <wx/dcbuffer.h>
#include <wx/grid.h>

#include "macros.h"
#include "ui/TracePropertyDialog.hh"

wxDEFINE_EVENT(WAVE_DISPLAY_CHANGE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(WAVE_CURSOR_MOVE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(WAVE_SELECTION_CHANGE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(SIGNAL_SELECTION_CHANGE_EVENT, wxCommandEvent);

static const Time marker_gap_list[] = {
    Time(1),
    Time(2),
    Time(5),
    Time(10),
    Time(20),
    Time(50),
    Time(100),
    Time(200),
    Time(500),
    Time(1'000),
    Time(2'000),
    Time(5'000),
    Time(10'000),
    Time(20'000),
    Time(50'000),
    Time(100'000),
    Time(200'000),
    Time(500'000),
    Time(1'000'000),
    Time(2'000'000),
    Time(5'000'000),
    Time(10'000'000),
    Time(20'000'000),
    Time(50'000'000),
    Time(100'000'000),
    Time(200'000'000),
    Time(500'000'000),
    Time(1'000'000'000),
    Time(2'000'000'000),
    Time(5'000'000'000),
    Time(10'000'000'000),
    Time(20'000'000'000),
    Time(50'000'000'000),
    Time(100'000'000'000),
    Time(200'000'000'000),
    Time(500'000'000'000),
    Time(1'000'000'000'000),
    Time(2'000'000'000'000),
    Time(5'000'000'000'000),
};

WaveViewerControl::WaveViewerControl(wxWindow* parent, wxWindowID winid,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxPanel(parent, winid, pos, size, style, name)
    , display_start(0)
    , display_period(1'000'000'000)
    , end_time(0)
    , wave_sel_start(-1)
    , wave_sel_end(-1)
    , wave_cursor_pos(-1)
    , root_trace_node("root", nullptr, true)
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
EVT_RIGHT_DOWN(WaveViewerControl::onRightDown)
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
    int row_height = node.GetRenderHeight();
    if (row_height < 20)
        row_height = 20;

    if (base_y <= mouse_y && mouse_y <= base_y + row_height + row_gap * 2) {
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
    bool isdark = wxSystemSettings::GetAppearance().IsDark();

    base_y += row_gap;
    int row_height = node.GetRenderHeight();
    if (row_height < 20)
        row_height = 20;

    // draw node name
    {
        wxRect name_rect(
            signal_list_area.x, base_y, signal_list_area.width, row_height);
        wxDCClipper clip(dc, name_rect.Intersect(signal_list_area));
        wxSize textSize = dc.GetTextExtent(node.GetName());

        dc.SetPen(*wxTRANSPARENT_PEN);
        if (&node == selection) {
            dc.SetBrush(wxColour(isdark ? 0x444444 : 0xBBBBBB));
        } else {
            dc.SetBrush(wxColour(isdark ? 0x222222 : 0xDDDDDD));
        }
        dc.DrawRectangle(name_rect);

        dc.DrawText(node.GetName(),
            wxPoint(20 + hie_level * 10,
                base_y + (row_height - textSize.GetHeight()) / 2));

        if (node.IsGroup()) {
            dc.SetBrush(wxColour(isdark ? 0xFFFFFF : 0x000000));

            if (node.IsFolded()) {
                dc.DrawPolygon(3, fold_handle_folded, 4 + hie_level * 10,
                    base_y + row_height / 2 - 5);
            } else {
                dc.DrawPolygon(3, fold_handle_unfolded, 4 + hie_level * 10,
                    base_y + row_height / 2 - 2);
            }
        }
    }

    if (node.GetTraceHandle()) {
        // draw trace
        if (base_y + row_height >= wave_area.y) {
            wxRect trace_rect(wave_area.x, base_y, wave_area.width, row_height);
            this->drawTrace(dc, node.GetTraceHandle(), trace_rect);
        }
    }

    base_y += row_height + row_gap;

    if (base_y >= wave_area.y) {
        dc.SetPen(wxColour(0x800000));
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

void WaveViewerControl::drawTrace(wxDC& dc,
    std::shared_ptr<Trace> trace_handle, const wxRect& rect)
{
    wxDCClipper clip(dc, rect.Intersect(wave_area));

    trace_handle->setCurrentTime(this->display_start);

    Time time = trace_handle->getCurrentTime();
    char apchar;
    Trace::AppearanceType aptype;
    double start_xpos, end_xpos;
    std::vector<wxPoint> point_list;

    do {
        if (time < display_start) continue;

        apchar = trace_handle->getAppearanceChar();
        aptype = trace_handle->getAppearanceType();

        start_xpos = (double)(time - display_start).steps
            / display_period.steps * rect.width;
        if (!trace_handle->seekNextTransition()) {
            time = this->end_time;
        } else {
            time = trace_handle->getCurrentTime();
        }
        end_xpos = (double)(time - display_start).steps
            / display_period.steps * rect.width;

        point_list.push_back(wxPoint(start_xpos, rect.height / 2));
        switch (aptype) {
            case Trace::AT_LOW:
                point_list.push_back(wxPoint(start_xpos + 1, rect.height - 1));
                point_list.push_back(wxPoint(end_xpos - 1, rect.height - 1));
                break;
            case Trace::AT_MID:
                point_list.push_back(wxPoint(start_xpos + 1, rect.height / 2));
                point_list.push_back(wxPoint(end_xpos - 1, rect.height / 2));
                break;
            case Trace::AT_HIGH:
            case Trace::AT_BOTH:
            case Trace::AT_UNKNOWN:
                point_list.push_back(wxPoint(start_xpos + 1, 0));
                point_list.push_back(wxPoint(end_xpos - 1, 0));
                break;
            default:
                break;
        }
        point_list.push_back(wxPoint(end_xpos, rect.height / 2));
        switch (aptype) {
            case Trace::AT_BOTH:
            case Trace::AT_UNKNOWN:
                point_list.push_back(wxPoint(end_xpos - 1, rect.height - 1));
                point_list.push_back(wxPoint(start_xpos + 1, rect.height - 1));
                point_list.push_back(wxPoint(start_xpos, rect.height / 2));
                break;
            default:
                break;
        }

        if (end_xpos - start_xpos >= 20) {
            wxColour textcolor = dc.GetTextForeground();
            dc.SetTextForeground(wxColour(0xEEEEEE));
            dc.DrawText(wxString(apchar), wxPoint(rect.x + start_xpos + 4, rect.y + 2));
            dc.SetTextForeground(textcolor);
        }
        
        dc.SetPen(*wxGREEN_PEN);
        dc.DrawLines(point_list.size(), point_list.data(), rect.x, rect.y);
        point_list.clear();
    } while (time < this->end_time);
}

void WaveViewerControl::onResize(wxSizeEvent& event) { this->calculateAreas(); }

static const Time::timeunit timeunit_table[] = {
    Time::FS, Time::PS, Time::NS, Time::US, Time::MS, Time::S,
};

static const std::string timeunit_str_table[] = {
    "fs", "ps", "ns", "us", "ms", "s",
};

void WaveViewerControl::onPaint(wxPaintEvent&)
{
    wxPaintDC dc(this);

    // is dark mode?
    bool isdark = wxSystemSettings::GetAppearance().IsDark();

    // fill area
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(time_indicator_area);
    dc.DrawRectangle(wave_area);

    dc.SetBrush(wxColour(isdark ? 0x111111 : 0xEEEEEE));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(signal_list_area);

    // draw time indicator / grid column
    dc.SetPen(*wxGREY_PEN);
    dc.DrawLine(time_indicator_area.GetLeftBottom(),
        time_indicator_area.GetRightBottom());
    Time marker_time = display_start;
    Time marker_gap = marker_gap_list[ARRAY_SIZE(marker_gap_list) - 1];

    for (int i = 0; i < ARRAY_SIZE(marker_gap_list); i++) {
        int marker_count = display_period.steps / marker_gap_list[i].steps;
        if (2 < marker_count && marker_count < 8) {
            marker_gap = marker_gap_list[i];
            break;
        }
    }

    marker_time.steps -= marker_time.steps % marker_gap.steps;
    if (marker_time < display_start)
        marker_time.steps += marker_gap.steps;

    while (marker_time < display_start + display_period
        && marker_time <= end_time) {
        double xpos = (double)(marker_time - display_start).steps
            / display_period.steps * wave_area.GetWidth();

        {
            wxDCClipper clip(dc, time_indicator_area);

            int timeunit_idx = (int)floor((log10(display_period.steps) - 1) / 3);
            if (timeunit_idx < 0) timeunit_idx = 0;
            if (timeunit_idx > 5) timeunit_idx = 5;
            Time::timeunit timeunit = timeunit_table[timeunit_idx];
            std::string timeunit_str = timeunit_str_table[timeunit_idx];
            
            std::string marker_text = marker_time.steps
                ? std::to_string((int)marker_time.getTime(timeunit)) + " " + timeunit_str
                : "0";
            wxSize text_size = dc.GetTextExtent(marker_text);

            dc.SetPen(wxColour(0xEEEEEE));
            dc.DrawLine(wxPoint(time_indicator_area.GetX() + xpos, 18),
                wxPoint(time_indicator_area.GetX() + xpos,
                    time_indicator_area.GetBottom()));
            wxColour textcolor = dc.GetTextForeground();
            dc.SetTextForeground(wxColour(0xEEEEEE));
            dc.DrawText(marker_text,
                wxPoint(time_indicator_area.GetX() + xpos
                        - text_size.GetWidth() / 2,
                    time_indicator_area.GetTop() + 4));
            dc.SetTextForeground(textcolor);
        }

        dc.SetPen(wxColour(0x800000));
        dc.DrawLine(
            wxPoint(time_indicator_area.GetX() + xpos, wave_area.GetTop()),
            wxPoint(time_indicator_area.GetX() + xpos, wave_area.GetBottom()));

        marker_time.steps += marker_gap.steps;
    }

    // draw wave
    this->drawNode(dc, this->root_trace_node, wave_area.y - vscroll_offset);

    // draw cursor and selection
    dc.SetPen(*wxGREY_PEN);
    int xpos;
    if (display_start <= wave_cursor_pos
        && wave_cursor_pos <= display_start + display_period) {
        xpos = timeToPos(wave_cursor_pos);
        dc.DrawLine(wxPoint(xpos, wave_area.GetTop()),
            wxPoint(xpos, wave_area.GetBottom()));
    }

    dc.SetPen(*wxBLUE_PEN);
    if (display_start <= wave_sel_start
        && wave_sel_start <= display_start + display_period) {
        xpos = timeToPos(wave_sel_start);
        dc.DrawLine(wxPoint(xpos, wave_area.GetTop()),
            wxPoint(xpos, wave_area.GetBottom()));
    }
    if (display_start <= wave_sel_end
        && wave_sel_end <= display_start + display_period) {
        xpos = timeToPos(wave_sel_end);
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

        wxCommandEvent new_event(WAVE_SELECTION_CHANGE_EVENT, GetId());
        new_event.SetEventObject(this);
        ProcessWindowEvent(new_event);
    } else if (signal_list_area.Contains(mouse_pos)) {
        auto result = this->findNodeByMousePos(
            mouse_pos.y, this->root_trace_node, wave_area.y - vscroll_offset);

        if (result.first != nullptr
            && mouse_pos.y >= result.second + result.first->GetRenderHeight()
            && result.first->GetTraceHandle() != nullptr) {
            resizing_row = true;
            resize_node_ypos = result.second;
            resize_target_node = result.first;
            this->SetCursor(wxCURSOR_SIZENS);
        } else if (this->selection != result.first) {
            this->selection = result.first;

            wxCommandEvent new_event(SIGNAL_SELECTION_CHANGE_EVENT, GetId());
            new_event.SetEventObject(this);
            ProcessWindowEvent(new_event);
        }
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

        if (this->selection != result.first) {
            this->selection = result.first;

            wxCommandEvent new_event(SIGNAL_SELECTION_CHANGE_EVENT, GetId());
            new_event.SetEventObject(this);
            ProcessWindowEvent(new_event);
        }

        if (result.first != nullptr) {
            if (result.first->IsGroup()) {
                result.first->SetFoldStatus(!result.first->IsFolded());
            } else if (mouse_pos.y
                >= result.second + result.first->GetRenderHeight()) {
                result.first->SetRenderHeight(20);
            } else {
                TracePropertyDialog dialog(this);

                dialog.ShowModal();
            }
        }
    } else if (wave_area.Contains(mouse_pos)) {
        wave_sel_end = wave_sel_start = Time(-1);
    }

    this->Refresh(false);
}

void WaveViewerControl::onLeftUp(wxMouseEvent& event)
{
    if (resizing_signal_list || resizing_row) {
        this->resizing_signal_list = false;
        this->resizing_row = false;
        this->SetCursor(wxCURSOR_ARROW);
    }
}

void WaveViewerControl::onRightDown(wxMouseEvent& event)
{
    SetFocus();

    wxPoint mouse_pos = event.GetPosition();

    if (wave_area.Contains(mouse_pos)) {
    } else if (signal_list_area.Contains(mouse_pos)) {
        auto result = this->findNodeByMousePos(
            mouse_pos.y, this->root_trace_node, wave_area.y - vscroll_offset);

        if (result.first != nullptr
            && mouse_pos.y <= result.second + result.first->GetRenderHeight()
            && result.first->GetTraceHandle() != nullptr) {
            wxMenu menu;

            menu.Append(wxID_UNDO, wxT("&Undo"));
            menu.Append(wxID_REDO, wxT("&Redo"));
            menu.AppendSeparator();
            menu.Append(wxID_CUT, wxT("&Cut"));
            menu.Append(wxID_COPY, wxT("&Copy"));
            menu.Append(wxID_PASTE, wxT("&Paste"));
            menu.Append(wxID_DELETE, wxT("&Delete"));
            menu.AppendSeparator();
            menu.Append(wxID_EDIT, wxT("&Edit Trace Properties"));
            Bind(wxEVT_MENU, &WaveViewerControl::onPopupMenuItemEditTracePropertiesClicked, this, wxID_EDIT);
            PopupMenu(&menu, mouse_pos);
        }
    }

    this->Refresh(false);
}

void WaveViewerControl::onMouseLeave(wxMouseEvent& event)
{
    if (resizing_signal_list || resizing_row) {
        this->resizing_signal_list = false;
        this->resizing_row = false;
        this->SetCursor(wxCURSOR_ARROW);
    }
}

void WaveViewerControl::onMouseMotion(wxMouseEvent& event)
{
    wxPoint mouse_pos = event.GetPosition();

    if (resizing_signal_list && mouse_pos.x > 80) {
        signal_list_width = mouse_pos.x - separator_width / 2;
        this->calculateAreas();
    } else if (resizing_row && resize_target_node
        && resize_node_ypos + 20 <= mouse_pos.y) {
        resize_target_node->SetRenderHeight(mouse_pos.y - resize_node_ypos);
    } else if (wave_area.Contains(mouse_pos)) {
        if (event.Dragging()) {
            this->wave_sel_end = this->posToTime(mouse_pos.x);
        }
        this->wave_cursor_pos = this->posToTime(mouse_pos.x);

        wxCommandEvent new_event(WAVE_CURSOR_MOVE_EVENT, GetId());
        new_event.SetEventObject(this);
        ProcessWindowEvent(new_event);
    } else if (signal_list_area.Contains(mouse_pos)) {
        auto result = this->findNodeByMousePos(
            mouse_pos.y, this->root_trace_node, wave_area.y - vscroll_offset);

        if (result.first != nullptr
            && mouse_pos.y >= result.second + result.first->GetRenderHeight()
            && result.first->GetTraceHandle() != nullptr) {
            this->SetCursor(wxCURSOR_SIZENS);
        } else {
            this->SetCursor(wxCURSOR_ARROW);
        }
    } else {
        this->SetCursor(wxCURSOR_ARROW);
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

        wxCommandEvent new_event(WAVE_DISPLAY_CHANGE_EVENT, GetId());
        new_event.SetEventObject(this);
        ProcessWindowEvent(new_event);

        wxCommandEvent new_event2(WAVE_CURSOR_MOVE_EVENT, GetId());
        new_event2.SetEventObject(this);
        ProcessWindowEvent(new_event2);
    } else if (event.GetWheelAxis()
        == wxMouseWheelAxis::wxMOUSE_WHEEL_VERTICAL) {
        this->vscroll_offset -= event.GetWheelRotation();
        if (this->vscroll_offset < 0) {
            this->vscroll_offset = 0;
        }
    }

    this->Refresh(false);
}

void WaveViewerControl::onPopupMenuItemUndoClicked(wxCommandEvent& event)
{

}

void WaveViewerControl::onPopupMenuItemRedoClicked(wxCommandEvent& event)
{

}

void WaveViewerControl::onPopupMenuItemCutClicked(wxCommandEvent& event)
{

}

void WaveViewerControl::onPopupMenuItemCopyClicked(wxCommandEvent& event)
{

}

void WaveViewerControl::onPopupMenuItemPasteClicked(wxCommandEvent& event)
{

}

void WaveViewerControl::onPopupMenuItemDeleteClicked(wxCommandEvent& event)
{

}

void WaveViewerControl::onPopupMenuItemEditTracePropertiesClicked(wxCommandEvent& event)
{
    TracePropertyDialog* dialog = new TracePropertyDialog(this);

    dialog->ShowModal();
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
        + (double)(time - display_start).steps / display_period.steps
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

    wxCommandEvent new_event(WAVE_DISPLAY_CHANGE_EVENT, GetId());
    new_event.SetEventObject(this);
    ProcessWindowEvent(new_event);
}

Time WaveViewerControl::GetDisplayPeriod() const
{
    return this->display_period;
}

void WaveViewerControl::SetDisplayPeriod(Time period)
{
    this->display_period = period;
    this->Refresh(false);

    wxCommandEvent new_event(WAVE_DISPLAY_CHANGE_EVENT, GetId());
    new_event.SetEventObject(this);
    ProcessWindowEvent(new_event);
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
