#pragma once

namespace gui
{

// Forward declaration.
class client_panel;

class client_frame: public wxFrame
{
	// Construction and destruction.
public:
	// Constructor.
	client_frame();
	// Destructor.
	~client_frame() = default;

	// Public types.
public:
	// Frame panel pointer type.
	using client_panel_ptr = client_panel*;

	// Private interface.
private:
	// Menu event handler.
	void OnMenuEvent(wxCommandEvent& e);
	// On close event handler
	void OnClose(wxCloseEvent& e);

	// Private members.
private:
	// Panel pointer.
	client_panel_ptr panel_;
};

};