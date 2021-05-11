#pragma once

namespace gui
{

// Forward declaration.
class client_panel;

// Define custom command event
class netlib_event;
wxDECLARE_EVENT(NETLIB_EVENT_TYPE, netlib_event);
class netlib_event : public wxCommandEvent {
public:
	netlib_event(wxEventType commandType = NETLIB_EVENT_TYPE, int id = 0)
		: wxCommandEvent(commandType, id) {
	}
	// You *must* copy here the data to be transported
	netlib_event(const netlib_event& event)
		: wxCommandEvent(event) {
		this->width_ = event.width_;
		this->height_ = event.height_;
	}
	// Required for sending with wxPostEvent()
	wxEvent* Clone() const {
		return new netlib_event(*this);
	}
	int width_;
	int height_;
};

typedef void (wxEvtHandler::* netlib_event_function)(netlib_event&);
#define netlib_event_handler(func) wxEVENT_HANDLER_CAST(netlib_event_function, func)
#define EVT_MYFOO(id, func) wx__DECLARE_EVT1(NETLIB_EVENT_TYPE, id, netlib_event_handler(func))

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
	// netlib event type
	enum netlib_event_t {
		NETLIB_FULLSCREEN = 99
	};

	// Private interface.
private:
	// On close event handler
	void OnClose(wxCloseEvent& e);

	// netlib session connected
	void on_session_connected(const netlib::netlib_session::sessionid_t&);
	// netlib session disconnected
	void on_session_disconnected(const netlib::netlib_session::sessionid_t& id);
	// netlib session data received
	void on_session_received(
		const netlib::netlib_session::sessionid_t&,
		const netlib::netlib_message::buff_t::value_type*,
		const std::size_t);
	// netlib event handler
	void on_netlib_event_handler(netlib_event& e);

	// Any class wishing to process wxWidgets events must use this macro.
	wxDECLARE_EVENT_TABLE();

	// Private members.
private:
	// Panel pointer.
	client_panel_ptr panel_;
	// netlib pointer
	std::shared_ptr<netlib::netlib_client> netlib_client_;
};

};