#pragma once

namespace gui
{

class client_panel : public wxPanel
{
	// Construction and destruction.
public:
	// Constructor.
	client_panel(wxWindow* parent);
	// Destructor.
	~client_panel() = default;

	// Public interface
public:
	// Update screen
	void update_screen(const wxImage& img, const wxRect& r, bool is_fullscreen);
	// Render
	void render(wxDC& dc);

	// Paint event handler
	void on_paint(wxPaintEvent&);

	// Public interface.
private:
	// Any class wishing to process wxWidgets events must use this macro.
	wxDECLARE_EVENT_TABLE();

	// Private members.
private:
	// Image
	wxImage image_;
};

};