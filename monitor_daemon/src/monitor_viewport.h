namespace screen {

class viewport : public netlib::netlib_core {
	// Public types
public:
	using do_send = std::function<void(const netlib::netlib_session::sessionid_t& id, const netlib::viewport::Frame& frame)>;
	using bytes = std::vector<unsigned char>;
	enum algorithm_t {
		VIEWPORT_DIFF_SIMPLE = 99,
		VIEWPORT_DIFF_CHUNKED
	};

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
	// Checks whether two pixels are equal
	static bool are_pixels_equal(const wxImage& current, const wxImage& previous, const int x, const int y);

	// Public interface
public:
	// Set viewport algorithm
	void set_algorithm(const algorithm_t& algorithm);

	// Public members
public:
	// Primary display sizes
	static wxRect primary_display_;

	// Private interface
private:
	// Screen diff handler
	void screen_diff_handler(const boost::system::error_code& e);
	// Screen diff handler chunked
	void screen_diff_handler_chunked(const boost::system::error_code& ec);
	// Pack and send screen data
	void screen_data_pack_and_send(const wxImage& img, const wxRect& rect) const;
	// Set screen diff handler
	void set_screen_diff_handler();

private:
	// Session id to send to
	netlib::netlib_session::sessionid_t id_;
	// Previous desktop view
	wxImage previous_;
	// Callback to send diff outside
	do_send send_handler_;
	// Diff handler timer
	boost::asio::steady_timer timer_;
	// Currently using algorithm
	algorithm_t algorithm_;
};

}