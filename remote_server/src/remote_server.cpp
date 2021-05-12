#include "remote_server.h"

#define MONITOR_POCO_LOGGER_NAME "Application"
#define MONITOR_POCO_LOGGER_SOURCE "Monitor"

#define LOG_INF(msg)														\
Poco::Logger::get(MONITOR_POCO_LOGGER_NAME).log(Poco::Message(				\
		MONITOR_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_INFORMATION))	\

#define LOG_INF_FMT(fmt, ...)												\
		NETLIB_FTL(Poco::format(fmt, ##__VA_ARGS__))						\

#define LOG_ERR(msg)														\
Poco::Logger::get(MONITOR_POCO_LOGGER_NAME).log(Poco::Message(				\
		MONITOR_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_ERROR))		\

#define LOG_ERR_FMT(fmt, ...)												\
		NETLIB_FTL(Poco::format(fmt, ##__VA_ARGS__))						\

#define LOG_WHAT2STR(e) std::string(e.what())

namespace server {

remote_server::remote_server()
	: Poco::Util::ServerApplication()
	, netlib_server_(nullptr)
	, viewport_(nullptr) {
}

int remote_server::main(const ArgVec& args) {

	try {
		wxImage::AddHandler(new wxPNGHandler);

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
						if (frame.fullscreen()) {
							wxImage img = viewport::make_screenshot_image();
							viewport::bytes container;
							viewport::save_image_to_container(img, container);
							frame.set_data(&container[0], container.size());
							frame.set_width(viewport::primary_display_.GetWidth());
							frame.set_height(viewport::primary_display_.GetHeight());

							send(id, frame);
							viewport::do_send do_send = std::bind(&remote_server::send, this, std::placeholders::_1, std::placeholders::_2);
							viewport_ = std::make_shared<viewport>(id, img, do_send);
							viewport_->start();
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
	, timer_(io_context_) {
	run();
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
				if (current.GetRed(x, y) != previous_.GetRed(x, y) ||
					current.GetGreen(x, y) != previous_.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous_.GetBlue(x, y)) {
					rect.SetTop(y);
					done = true;
					break;
				}
			}
			if (done) {
				break;
			}
		}

		done = false;
		for (int x = 0; x < current.GetWidth(); ++x) {
			for (int y = 0; y < current.GetHeight(); ++y) {
				if (current.GetRed(x, y) != previous_.GetRed(x, y) ||
					current.GetGreen(x, y) != previous_.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous_.GetBlue(x, y)) {
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
			for (int x = current.GetWidth() - 1; x >= 0 ; --x) {
				if (current.GetRed(x, y) != previous_.GetRed(x, y) ||
					current.GetGreen(x, y) != previous_.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous_.GetBlue(x, y)) {
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
				if (current.GetRed(x, y) != previous_.GetRed(x, y) ||
					current.GetGreen(x, y) != previous_.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous_.GetBlue(x, y)) {
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
			//static int count = 0;
			wxImage sub = current.GetSubImage(rect);
			previous_.Destroy();
			previous_ = current;
			viewport::bytes container;
			viewport::save_image_to_container(sub, container);
			netlib::viewport::Frame frame;
			frame.set_fullscreen(false);
			frame.set_width(rect.GetWidth());
			frame.set_height(rect.GetHeight());
			frame.set_x(rect.GetX());
			frame.set_y(rect.GetY());
			frame.set_data(&container[0], container.size());
			//sub.SaveFile(Poco::format("img\\sub_image%d.png", count), wxBITMAP_TYPE_PNG);
			//count++;
			send_handler_(id_, frame);
		} else {
			LOG_INF("Nothing to send...");
		}
	}

	timer_.expires_after(std::chrono::milliseconds(300));
	timer_.async_wait(std::bind(&viewport::screen_diff_handler, this, std::placeholders::_1));
}

void viewport::start() {
	timer_.expires_after(std::chrono::milliseconds(1000));
	timer_.async_wait(std::bind(&viewport::screen_diff_handler, this, std::placeholders::_1));
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
	wxBitmap screenShot(primary_display_.GetWidth(), primary_display_.GetHeight(), 24);
	wxMemoryDC memDC;
	memDC.SelectObject(screenShot);
	wxScreenDC dcScreen;
	memDC.Blit(0, 0, screenShot.GetWidth(), screenShot.GetHeight(), &dcScreen, 0, 0, wxRasterOperationMode(wxCOPY), true);
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

}