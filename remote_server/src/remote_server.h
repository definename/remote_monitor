#ifndef REMOTE_SERVER
#define REMOTE_SERVER

namespace server {

class viewport : public netlib::netlib_core {
	// Public types
public:
	using do_send = std::function<void(const netlib::netlib_session::sessionid_t& id, const netlib::viewport::Frame& frame)>;
	using bytes = std::vector<unsigned char>;

public:
	// Constructor
	viewport(
		const netlib::netlib_session::sessionid_t& id,
		wxImage& previous,
		viewport::do_send& send_handler);
	// Start
	void start();
	// Stop
	void stop();

	// Make screenshot image
	static wxImage make_screenshot_image();
	// Save image to container to transfer
	static void save_image_to_container(const wxImage& img, viewport::bytes& container);

	// Public members
public:
	// Primary display sizes
	static wxRect primary_display_;

	// Private interface
private:
	// Screen diff handler
	void screen_diff_handler(const boost::system::error_code& e);

private:
	// Session id to send to
	netlib::netlib_session::sessionid_t id_;
	// Previous desktop view
	wxImage previous_;
	// Callback to send diff outside
	do_send send_handler_;
	// Diff handler timer
	boost::asio::steady_timer timer_;
};

class remote_server: public Poco::Util::ServerApplication {
public:
	remote_server();
	int main(const ArgVec& args) override;

private:
	void send(const netlib::netlib_session::sessionid_t&, const netlib::viewport::Frame&);

private:
	std::shared_ptr<netlib::netlib_server> netlib_server_;
	std::shared_ptr<viewport> viewport_;
};

}

POCO_SERVER_MAIN(server::remote_server)

#endif