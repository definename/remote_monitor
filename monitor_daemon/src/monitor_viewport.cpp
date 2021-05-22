#include "monitor_viewport.h"

#define SCREEN_DEPTH 24

namespace screen {

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
			}
			else {
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

void viewport::screen_diff_handler(const boost::system::error_code & ec) {
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

void viewport::screen_data_pack_and_send(const wxImage & img, const wxRect & rect) const {
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
		}
		catch (const std::exception & e) {
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

void viewport::save_image_to_container(const wxImage & img, viewport::bytes & container) {
	wxMemoryOutputStream outStream;
	img.SaveFile(outStream, wxBITMAP_TYPE_PNG);
	wxMemoryInputStream inputStream(outStream);
	size_t size(inputStream.GetSize());
	container.resize(size);
	inputStream.Read(&container[0], size);
}

void viewport::set_algorithm(const algorithm_t & algorithm) {
	algorithm_ = algorithm;
}

void viewport::set_screen_diff_handler() {
	if (algorithm_ == algorithm_t::VIEWPORT_DIFF_SIMPLE) {
		timer_.async_wait(std::bind(&viewport::screen_diff_handler, this, std::placeholders::_1));
	}
	else if (algorithm_ == algorithm_t::VIEWPORT_DIFF_CHUNKED) {
		timer_.async_wait(std::bind(&viewport::screen_diff_handler_chunked, this, std::placeholders::_1));
	}
	else {
		LOG_ERR("Unknown algorithm");
	}
}

bool viewport::are_pixels_equal(const wxImage & current, const wxImage & previous, const int x, const int y) {
	if (current.GetRed(x, y) == previous.GetRed(x, y) ||
		current.GetGreen(x, y) == previous.GetGreen(x, y) ||
		current.GetBlue(x, y) == previous.GetBlue(x, y)) {
		return true;
	}
	return false;
}

}