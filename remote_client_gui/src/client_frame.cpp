#include "client_frame.h"
#include "client_panel.h"

namespace gui
{
wxDEFINE_EVENT(NETLIB_EVENT_TYPE, netlib_event);

wxBEGIN_EVENT_TABLE(client_frame, wxFrame)
	EVT_MYFOO(wxID_ANY, client_frame::on_netlib_event_handler)
wxEND_EVENT_TABLE()

client_frame::client_frame()
	: wxFrame(NULL, wxID_ANY, wxT("Monitor GUI"), wxDefaultPosition, wxSize(550, 300), wxDEFAULT_FRAME_STYLE)
	, panel_(new client_panel(this))
	, netlib_client_(std::make_shared<netlib::netlib_client>()) {

	SetIcon(wxICON(FRAME_ICON));

	Bind(wxEVT_CLOSE_WINDOW, &client_frame::OnClose, this);

	netlib_client_->connect(boost::asio::ip::host_name(), 49160);
	netlib_client_->do_on_session_connected(boost::bind(&client_frame::on_session_connected, this, _1));
	netlib_client_->do_on_session_disconnected(boost::bind(&client_frame::on_session_disconnected, this, _1));
	netlib_client_->do_on_session_received(boost::bind(&client_frame::on_session_received, this, _1, _2, _3));
}

void client_frame::OnClose(wxCloseEvent& WXUNUSED(e)) {
	netlib_client_->stop();
	Destroy();
}

void client_frame::on_session_connected(const netlib::netlib_session::sessionid_t& id) {
	try {
		netlib::viewport::Frame frame;
		frame.set_fullscreen(true);
		netlib_client_->send(id, frame);
	} catch (const std::exception & e) {
		//logger().error("Failed to send frame:", WHAT_TO_STR(e));
	}
}

void client_frame::on_session_disconnected(const netlib::netlib_session::sessionid_t& id) {
	//logger().information("Disconnected...");
}

void client_frame::on_session_received(
	const netlib::netlib_session::sessionid_t& id,
	const netlib::netlib_message::buff_t::value_type* data,
	const std::size_t bytes) {
	try {
		netlib::viewport::Frame frame;
		if (frame.ParseFromArray(data, bytes)) {
			netlib_event e(NETLIB_EVENT_TYPE, netlib_event_t::NETLIB_SUB_IMAGE);
			if (frame.fullscreen()) {
				e.SetId(netlib_event_t::NETLIB_FULLSCREEN);
			}
			e.width_ = frame.width();
			e.height_ = frame.height();
			e.x_ = frame.x();
			e.y_ = frame.y();

			wxMemoryInputStream is(reinterpret_cast<const void*>(frame.data().data()), frame.data().size());
			e.img_ = wxImage(is, wxBITMAP_TYPE_PNG);
			wxPostEvent(this, e);
		} else {
			//logger().error("Failed to parse with protocol buffer");
		}
	}
	catch (const std::exception & e) {
		//logger().error(Poco::format("Receiving handler error:%s", WHAT_TO_STR(e)));
	}
}

void client_frame::on_netlib_event_handler(netlib_event& e) {
	if (e.GetId() == netlib_event_t::NETLIB_FULLSCREEN) {
		SetClientSize(e.width_, e.height_);
	}

	wxRect r;
	r.SetWidth(e.width_);
	r.SetHeight(e.height_);
	r.SetX(e.x_);
	r.SetY(e.y_);
	panel_->update_screen(e.img_, r, e.GetId() == netlib_event_t::NETLIB_FULLSCREEN);
}

}
