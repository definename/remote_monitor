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
		netlib_server_ = std::make_shared<netlib::netlib_server>(49160);
		netlib_server_->do_on_session_connected([this](const netlib::netlib_session::sessionid_t& id) {
			logger().information("Connected...");
			});
		netlib_server_->do_on_session_disconnected([this](const netlib::netlib_session::sessionid_t& id) {
			logger().information("Disconnected..."); });
		netlib_server_->do_on_session_received([this](
			const netlib::netlib_session::sessionid_t& id,
			const netlib::netlib_message::buff_t::value_type* data,
			const std::size_t bytes) {
				try {
					logger().information("Data received...");
					netlib::viewport::Frame frame;
					if (!frame.ParseFromArray(data, bytes)) {
						logger().error("Failed to parse with protocol buffer");
					}
					logger().information(Poco::format("%s", frame.a()));
					frame.set_a("Wellcome!!!");
					netlib_server_->send(id, frame);
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