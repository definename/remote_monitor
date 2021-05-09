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

		waitForTerminationRequest();

	} catch (const std::exception& e) {
		logger().error("Error occurred: %s", e.what());
	}
	return Application::EXIT_OK;
}

}