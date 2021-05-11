#include "remote_client.h"

namespace server {

remote_client::remote_client()
: Poco::Util::ServerApplication()
, netlib_client_(nullptr) {
}

int remote_client::main(const ArgVec& args) {

	try {
		logger().setLevel("trace");
		Poco::AutoPtr<Poco::PatternFormatter> formatter =
			new Poco::PatternFormatter("%L%Y-%m-%d %H:%M:%S.%i %s [%p] %t");
		Poco::AutoPtr<Poco::FormattingChannel> channel =
			new Poco::FormattingChannel(formatter, logger().getChannel());
		logger().setChannel(channel);

		logger().information("Client is being started...");
		netlib_client_ = std::make_shared<netlib::netlib_client>();
		netlib_client_->connect(boost::asio::ip::host_name(), 49160);
		netlib_client_->do_on_session_connected([this](const netlib::netlib_session::sessionid_t& id) {
			try {
// 				logger().information("Connected...");
// 				netlib::viewport::Frame frame;
// 				frame.set_a("Hello!!!");
// 				netlib_client_->send(id, frame);
			} catch (const std::exception& e) {
				logger().error("Failed to send frame:", WHAT_TO_STR(e));
			}
			});
		netlib_client_->do_on_session_disconnected([this](const netlib::netlib_session::sessionid_t& id) {
			logger().information("Disconnected..."); });
		netlib_client_->do_on_session_received([this](const netlib::netlib_session::sessionid_t& id, const netlib::netlib_message::buff_t::value_type* data, const std::size_t bytes) {
			try {
// 				logger().information("Data received...");
// 				netlib::viewport::Frame frame;
// 				if (!frame.ParseFromArray(data, bytes)) {
// 					logger().error("Failed to parse with protocol buffer");
// 				}
// 				logger().information(Poco::format("%s", frame.a()));
// 				frame.set_a("Hello!!!");
// 				std::this_thread::sleep_for(std::chrono::seconds(1));
// 				netlib_client_->send(id, frame);
			} catch (const std::exception& e) {
				logger().error(Poco::format("Receiving handler error:%s", WHAT_TO_STR(e)));
			}
			});

		waitForTerminationRequest();

	} catch (const std::exception& e) {
		logger().error("Error occurred: %s", e.what());
	}
	return Application::EXIT_OK;
}

}