#include "remote_server.h"

#define MONITOR_POCO_LOGGER_NAME "Application"
#define MONITOR_POCO_LOGGER_SOURCE "Monitor"

#define LOG_INF(msg)														\
Poco::Logger::get(MONITOR_POCO_LOGGER_NAME).log(Poco::Message(				\
		MONITOR_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_INFORMATION))	\

#define LOG_INF_FMT(fmt, ...)												\
		LOG_INF(Poco::format(fmt, ##__VA_ARGS__))							\

#define LOG_ERR(msg)														\
Poco::Logger::get(MONITOR_POCO_LOGGER_NAME).log(Poco::Message(				\
		MONITOR_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_ERROR))		\

#define LOG_ERR_FMT(fmt, ...)												\
		LOG_ERR(Poco::format(fmt, ##__VA_ARGS__))							\

#define LOG_WHAT2STR(e) std::string(e.what())

#define SCREEN_DEPTH 24

namespace server {

remote_server::remote_server()
	: Poco::Util::ServerApplication()
	, netlib_server_(nullptr)
	, viewport_(nullptr) {
}

int remote_server::main(const ArgVec& args) {

	try {
		wxImage::AddHandler(new wxPNGHandler);
// 		wxImage::AddHandler(new wxBMPHandler);
// 		wxImage::AddHandler(new wxJPEGHandler);
// 		wxImage::AddHandler(new wxTIFFHandler);

		logger().setLevel("trace");
		Poco::AutoPtr<Poco::PatternFormatter> formatter =
			new Poco::PatternFormatter("%L%Y-%m-%d %H:%M:%S.%i %s [%p] %t");
		Poco::AutoPtr<Poco::FormattingChannel> channel =
			new Poco::FormattingChannel(formatter, logger().getChannel());
		logger().setChannel(channel);

		logger().information("Server is being started...");
		netlib_server_ = std::make_shared<netlib::netlib_server>(49160);
		netlib_server_->do_on_session_connected([this](const netlib::netlib_session::sessionid_t& id) {
			LOG_INF("Connected..."); });
		netlib_server_->do_on_session_disconnected([this](const netlib::netlib_session::sessionid_t& id) {
			LOG_INF("Disconnected...");
			viewport_->stop();
			viewport_.reset();
			viewport_ = nullptr;
			});
		netlib_server_->do_on_session_received([this](
			const netlib::netlib_session::sessionid_t& id,
			const netlib::netlib_message::buff_t::value_type* data,
			const std::size_t bytes) {
				try {
					LOG_INF("Data received...");
					netlib::viewport::Frame frame;
					if (frame.ParseFromArray(data, bytes)) {
						if (frame.type() == netlib::viewport::Frame_Type_Start) {
							wxImage img = viewport::make_screenshot_image();
							viewport::bytes container;
							viewport::save_image_to_container(img, container);
							frame.set_type(netlib::viewport::Frame_Type_Data);
							frame.set_data(&container[0], container.size());
							frame.set_width(viewport::primary_display_.GetWidth());
							frame.set_height(viewport::primary_display_.GetHeight());
							frame.set_fullscreen(true);

//							img.SaveFile("screen_image_png.png", wxBITMAP_TYPE_PNG);
// 							img.SaveFile("screen_image_bpm.bpm", wxBITMAP_TYPE_BMP);
// 							img.SaveFile("screen_image_jpeg.jpeg", wxBITMAP_TYPE_JPEG);
// 							img.SaveFile("screen_image_tiff.tiff", wxBITMAP_TYPE_TIFF);

							send(id, frame);
							viewport::do_send do_send = std::bind(&remote_server::send, this, std::placeholders::_1, std::placeholders::_2);
							viewport_ = std::make_shared<viewport>(id, img, do_send);
							viewport_->start();
						} else if (frame.type() == netlib::viewport::Frame_Type_Algorith_Diff_Chunked) {
							viewport_->set_algorithm(viewport::algorithm_t::VIEWPORT_DIFF_CHUNKED);
							LOG_INF("Change algorithm to diff chunked");
						} else if (frame.type() == netlib::viewport::Frame_Type_Algorith_Diff_Simple) {
							viewport_->set_algorithm(viewport::algorithm_t::VIEWPORT_DIFF_SIMPLE);
							LOG_INF("Change algorithm to diff simple");
						}
					} else {
						LOG_ERR("Failed to parse with protocol buffer");
					}
				} catch (const std::exception& e) {
					LOG_ERR_FMT("Receiving handler error:%s", LOG_WHAT2STR(e));
				}
			});

		waitForTerminationRequest();

	} catch (const std::exception& e) {
		LOG_ERR_FMT("Error occurred: %s", LOG_WHAT2STR(e));
	}
	return Application::EXIT_OK;
}

void remote_server::send(const netlib::netlib_session::sessionid_t&id, const netlib::viewport::Frame& frame) {
	netlib_server_->send<netlib::viewport::Frame>(id, frame);
}

wxRect viewport::primary_display_ = wxDisplay().GetGeometry();

viewport::viewport(const netlib::netlib_session::sessionid_t& id, wxImage& previous, viewport::do_send& send_handler)
	: netlib::netlib_core(2)
	, id_(id)
	, previous_(previous)
	, send_handler_(send_handler)
	, timer_(io_context_)
	, algorithm_(viewport::algorithm_t::VIEWPORT_DIFF_CHUNKED) {
	run();
}

void viewport::screen_diff_handler_chunked(
	const boost::system::error_code& ec) {
	if (ec == boost::asio::error::operation_aborted) {
		LOG_INF("Monitor exit...");
		return;
	}
	if (timer_.expiry() < boost::asio::steady_timer::clock_type::now()) {
		wxImage current = viewport::make_screenshot_image();
		int width = current.GetWidth();
		int parts = (width >= 200) ? 10 : 1;
		int part_size = width / parts;
		int residue = width % parts;

		bool is_diff = false;
		for (int i = 0; i < parts; ++i) {
			int offset = part_size * i;
			bool is_true = residue && (i == parts - 1);
			int part_width = (is_true) ? part_size + residue : part_size;

			bool done = false;
			wxRect rect;
			for (int y = 0; y < current.GetHeight(); ++y) {
				for (int x = 0; x < part_width; ++x) {
					if (!are_pixels_equal(
						current, previous_, x + offset, y)) {
						rect.SetTop(y);
						done = true;
						break;
					}
				}
				if (done) {
					break;
				}
			}
			if (!done) {
				LOG_INF_FMT("Nothing to send in part:%d ...", i);
				continue;
			}
			done = false;
			for (int x = 0; x < part_width; ++x) {
				for (int y = 0; y < current.GetHeight(); ++y) {
					if (!are_pixels_equal(
						current, previous_, x + offset, y)) {
						rect.SetLeft(x + offset);
						done = true;
						break;
					}
				}
				if (done) {
					break;
				}
			}
			done = false;
			for (int y = current.GetHeight() - 1; y >= 0; --y) {
				for (int x = part_width - 1; x >= 0; --x) {
					if (!are_pixels_equal(
						current, previous_, x + offset, y)) {
						rect.SetBottom(y);
						done = true;
						break;
					}
				}
				if (done) {
					break;
				}
			}
			done = false;
			for (int x = part_width - 1; x >= 0; x--) {
				for (int y = current.GetHeight() - 1; y >= 0; --y) {
					if (!are_pixels_equal(
						current, previous_, x + offset, y)) {
						rect.SetRight(x + offset);
						done = true;
						break;
					}
				}
				if (done) {
					break;
				}
			}

			if (rect != wxRect()) {
				screen_data_pack_and_send(
					current.GetSubImage(rect), rect);
				is_diff = true;
			} else {
				LOG_INF("Nothing to send...");
			}
		}
		if (is_diff) {
			previous_.Destroy();
			previous_ = current;
		}
	}
	start();
}

void viewport::screen_diff_handler(const boost::system::error_code& ec) {
	if (ec == boost::asio::error::operation_aborted) {
		LOG_INF("Monitor exit...");
		return;
	}

	if (timer_.expiry() < boost::asio::steady_timer::clock_type::now()) {
		wxImage current = viewport::make_screenshot_image();

		bool done = false;
		wxRect rect;
		for (int y = 0; y < current.GetHeight(); ++y) {
			for (int x = 0; x < current.GetWidth(); ++x) {
				if (!are_pixels_equal(current, previous_, x, y)) {
					rect.SetTop(y);
					done = true;
					break;
				}
			}
			if (done) {
				break;
			}
		}
		if (!done) {
			start();
			LOG_INF("Nothing to send...");
			return;
		}

		done = false;
		for (int x = 0; x < current.GetWidth(); ++x) {
			for (int y = 0; y < current.GetHeight(); ++y) {
				if (!are_pixels_equal(current, previous_, x, y)) {
					rect.SetLeft(x);
					done = true;
					break;
				}
			}
			if (done) {
				break;
			}
		}
		done = false;
		for (int y = current.GetHeight() - 1; y >= 0; --y) {
			for (int x = current.GetWidth() - 1; x >= 0; --x) {
				if (!are_pixels_equal(current, previous_, x, y)) {
					rect.SetBottom(y);
					done = true;
					break;
				}
			}
			if (done) {
				break;
			}
		}
		done = false;
		for (int x = current.GetWidth() - 1; x >= 0; x--) {
			for (int y = current.GetHeight() - 1; y >= 0; --y) {
				if (!are_pixels_equal(current, previous_, x, y)) {
					rect.SetRight(x);
					done = true;
					break;
				}
			}
			if (done) {
				break;
			}
		}

		if (rect != wxRect()) {
			screen_data_pack_and_send(current.GetSubImage(rect), rect);
			previous_.Destroy();
			previous_ = current;
		}
		else {
			LOG_INF("Nothing to send...");
		}
	}
	start();
}

void viewport::screen_data_pack_and_send(const wxImage& img, const wxRect& rect) const {
	//static int count = 0;
	viewport::bytes container;
	viewport::save_image_to_container(img, container);
	netlib::viewport::Frame frame;
	frame.set_width(rect.GetWidth());
	frame.set_height(rect.GetHeight());
	frame.set_x(rect.GetX());
	frame.set_y(rect.GetY());
	frame.set_data(&container[0], container.size());
	frame.set_type(netlib::viewport::Frame_Type_Data);
	//sub.SaveFile(Poco::format("img\\sub_image%d.png", count), wxBITMAP_TYPE_PNG);
	//count++;
	LOG_INF_FMT("Send image width:%d height:%d", rect.GetWidth(), rect.GetHeight());
	send_handler_(id_, frame);
}

void viewport::start() {
	timer_.expires_after(std::chrono::milliseconds(300));
	set_screen_diff_handler();
}

void viewport::stop() {
	if (is_running()) {
		try {
			timer_.cancel();
			netlib_core::stop();
		} catch (const std::exception& e) {
			LOG_ERR_FMT("Viewport destruction error:%s", LOG_WHAT2STR(e));
		}
	}
}

wxImage viewport::make_screenshot_image() {
	wxBitmap screenShot(
		primary_display_.GetWidth(),
		primary_display_.GetHeight(),
		SCREEN_DEPTH);
	wxMemoryDC memDC;
	memDC.SelectObject(screenShot);
	wxScreenDC dcScreen;
	memDC.Blit(
		0,
		0,
		screenShot.GetWidth(),
		screenShot.GetHeight(),
		&dcScreen,
		0,
		0,
		wxRasterOperationMode(wxCOPY),
		true);
	memDC.SelectObject(wxNullBitmap);
	return screenShot.ConvertToImage();
}

void viewport::save_image_to_container(const wxImage& img, viewport::bytes& container) {
	wxMemoryOutputStream outStream;
	img.SaveFile(outStream, wxBITMAP_TYPE_PNG);
	wxMemoryInputStream inputStream(outStream);
	size_t size(inputStream.GetSize());
	container.resize(size);
	inputStream.Read(&container[0], size);
}

void viewport::set_algorithm(const algorithm_t& algorithm) {
	algorithm_ = algorithm;
}

void viewport::set_screen_diff_handler() {
	if (algorithm_ == algorithm_t::VIEWPORT_DIFF_SIMPLE) {
		timer_.async_wait(std::bind(&viewport::screen_diff_handler, this, std::placeholders::_1));
	} else if (algorithm_ == algorithm_t::VIEWPORT_DIFF_CHUNKED) {
		timer_.async_wait(std::bind(&viewport::screen_diff_handler_chunked, this, std::placeholders::_1));
	} else {
		LOG_ERR("Unknown algorithm");
	}
}

bool viewport::are_pixels_equal(const wxImage& current, const wxImage& previous, const int x, const int y) {
	if (current.GetRed(x, y) == previous.GetRed(x, y) ||
		current.GetGreen(x, y) == previous.GetGreen(x, y) ||
		current.GetBlue(x, y) == previous.GetBlue(x, y)) {
		return true;
	}
	return false;
}

}