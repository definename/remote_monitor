#include "remote_server.h"

namespace server {

void RemoteServer::initialize(Poco::Util::Application& self) {
	logger().setLevel("trace");
	Poco::AutoPtr<Poco::PatternFormatter> formatter =
		new Poco::PatternFormatter("%L%Y-%m-%d %H:%M:%S.%i [%p] %t");
	Poco::AutoPtr<Poco::FormattingChannel> channel =
		new Poco::FormattingChannel(formatter, logger().getChannel());
	logger().setChannel(channel);

	ServerApplication::initialize(self);
}

int RemoteServer::main(const ArgVec& args) {
	waitForTerminationRequest();
	return Application::EXIT_OK;
}

}