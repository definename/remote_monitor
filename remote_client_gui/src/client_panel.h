#pragma once

//#include "netlib_session.h"

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

	// Public interface.
private:
	// Control event handler.
	//void OnControl(wxCommandEvent& e);

	// Any class wishing to process wxWidgets events must use this macro.
	wxDECLARE_EVENT_TABLE();

	// Private members.
private:
	// Image
	wxImage image_;
};

};