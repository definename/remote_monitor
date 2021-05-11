#include "remote_server.h"

namespace server {

remote_server::remote_server()
: Poco::Util::ServerApplication()
, netlib_server_(nullptr) {
}

int remote_server::main(const ArgVec& args) {

	try {
		logger().setLevel("trace");
		Poco::AutoPtr<Poco::PatternFormatter> formatter =
			new Poco::PatternFormatter("%L%Y-%m-%d %H:%M:%S.%i %s [%p] %t");
		Poco::AutoPtr<Poco::FormattingChannel> channel =
			new Poco::FormattingChannel(formatter, logger().getChannel());
		logger().setChannel(channel);

		logger().information("Server is being started...");
// 		netlib_server_ = std::make_shared<netlib::netlib_server>(49160);
// 		netlib_server_->do_on_session_connected([this](const netlib::netlib_session::sessionid_t& id) {
// 			logger().information("Connected...");
// 			});
// 		netlib_server_->do_on_session_disconnected([this](const netlib::netlib_session::sessionid_t& id) {
// 			logger().information("Disconnected..."); });
// 		netlib_server_->do_on_session_received([this](
// 			const netlib::netlib_session::sessionid_t& id,
// 			const netlib::netlib_message::buff_t::value_type* data,
// 			const std::size_t bytes) {
// 				try {
// 					logger().information("Data received...");
// 					netlib::viewport::Frame frame;
// 					if (!frame.ParseFromArray(data, bytes)) {
// 						logger().error("Failed to parse with protocol buffer");
// 					}
// 					logger().information(Poco::format("%s", frame.a()));
// 					frame.set_a("Welcome!!!");
// 					netlib_server_->send(id, frame);
// 				} catch (const std::exception& e) {
// 					logger().error(Poco::format("Receiving handler error:%s", WHAT_TO_STR(e)));
// 				}
// 			});

		// Take screenshot
		wxImage::AddHandler(new wxPNGHandler);

// 		wxDisplay primary_display;
// 		wxRect primary_rect = primary_display.GetGeometry();
// 		wxBitmap screenShot(primary_rect.GetWidth(), primary_rect.GetHeight(), 24);
// 		wxMemoryDC memDC;
// 		memDC.SelectObject(screenShot);
// 		wxScreenDC dcScreen;
// 		memDC.Blit(0, 0, screenShot.GetWidth(), screenShot.GetHeight(), &dcScreen, 0, 0, wxRasterOperationMode(wxCOPY), true);
// 		memDC.SelectObject(wxNullBitmap);
// 		screenShot.SaveFile("current.png", wxBITMAP_TYPE_PNG);

		wxImage current("current.png", wxBITMAP_TYPE_PNG);
		if (!current.IsOk()) {
			throw std::runtime_error("Failed to load img");
		}
		wxImage previous("previous.png", wxBITMAP_TYPE_PNG);
		if (!previous.IsOk()) {
			throw std::runtime_error("Failed to load img1");
		}

		if (current.GetWidth() != previous.GetWidth() || current.GetHeight() != previous.GetHeight()) {
			throw std::runtime_error("Different sizes");
		}

		logger().information(Poco::format("width=%d height=%d", current.GetWidth(), current.GetHeight()));

		bool done = false;
		wxRect rect;
		for (int y = 0; y < current.GetHeight(); ++y) {
			for (int x = 0; x < current.GetWidth(); ++x) {
				if (current.GetRed(x, y) != previous.GetRed(x, y) ||
					current.GetGreen(x, y) != previous.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous.GetBlue(x, y)) {
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
				if (current.GetRed(x, y) != previous.GetRed(x, y) ||
					current.GetGreen(x, y) != previous.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous.GetBlue(x, y)) {
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
				if (current.GetRed(x, y) != previous.GetRed(x, y) ||
					current.GetGreen(x, y) != previous.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous.GetBlue(x, y)) {
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
				if (current.GetRed(x, y) != previous.GetRed(x, y) ||
					current.GetGreen(x, y) != previous.GetGreen(x, y) ||
					current.GetBlue(x, y) != previous.GetBlue(x, y)) {
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
			wxImage sub = current.GetSubImage(rect);
			sub.SaveFile("diff.png", wxBITMAP_TYPE_PNG);

			previous.Paste(sub, rect.GetX(), rect.GetY());
			previous.SaveFile("like_current.png", wxBITMAP_TYPE_PNG);
		} else {
			logger().information("Equal");
		}

		//waitForTerminationRequest();

	} catch (const std::exception& e) {
		logger().error("Error occurred: %s", WHAT_TO_STR(e));
	}
	return Application::EXIT_OK;
}

}