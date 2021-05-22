#include "monitor_main.h"
#include "monitor_viewport.h"

namespace daemon {

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
							wxImage img = screen::viewport::make_screenshot_image();
							screen::viewport::bytes container;
							screen::viewport::save_image_to_container(img, container);
							frame.set_type(netlib::viewport::Frame_Type_Data);
							frame.set_data(&container[0], container.size());
							frame.set_width(screen::viewport::primary_display_.GetWidth());
							frame.set_height(screen::viewport::primary_display_.GetHeight());
							frame.set_fullscreen(true);

//							img.SaveFile("screen_image_png.png", wxBITMAP_TYPE_PNG);
// 							img.SaveFile("screen_image_bpm.bpm", wxBITMAP_TYPE_BMP);
// 							img.SaveFile("screen_image_jpeg.jpeg", wxBITMAP_TYPE_JPEG);
// 							img.SaveFile("screen_image_tiff.tiff", wxBITMAP_TYPE_TIFF);

							send(id, frame);
							screen::viewport::do_send do_send = std::bind(&remote_server::send, this, std::placeholders::_1, std::placeholders::_2);
							viewport_ = std::make_shared<screen::viewport>(id, img, do_send);
							viewport_->start();
						} else if (frame.type() == netlib::viewport::Frame_Type_Algorith_Diff_Chunked) {
							viewport_->set_algorithm(screen::viewport::algorithm_t::VIEWPORT_DIFF_CHUNKED);
							LOG_INF("Change algorithm to diff chunked");
						} else if (frame.type() == netlib::viewport::Frame_Type_Algorith_Diff_Simple) {
							viewport_->set_algorithm(screen::viewport::algorithm_t::VIEWPORT_DIFF_SIMPLE);
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

}