#pragma once

namespace gui
{

//! Forward declaration.
class client_frame;

class client_app : public wxApp
{
	// Construction and destruction.
public:
	// Constructor.
	client_app() = default;
	// Destructor.
	~client_app() = default;

	// Public types.
public:
	// Main frame pointer type.
	using client_frame_ptr = client_frame*;

	//
	// Public interface.
	//
public:
	// For any processing which needs to be done as the application is about to exit.
	virtual int OnExit() wxOVERRIDE;
	// Initializes the application.
	virtual bool OnInit(void) wxOVERRIDE;

	//
	// Private date members.
	//
private:
	// Frame pointer.
	client_frame_ptr frame_;
};

wxDECLARE_APP(client_app);

};