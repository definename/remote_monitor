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


// 		std::cout << "zlib version: " << zlib_version << std::endl;
// 
// 		std::string archive;
// 
// 		// Serialization.
// 		{
// 			Rich::Boss boss;
// 			boss.set_first_name("Ivan");
// 			boss.set_last_name("Durak");
// 
// 			for (int i = 0; i != 3; ++i)
// 			{
// 				auto slave = boss.add_slave();
// 				slave->set_number(i);
// 			}
// 
// 			if (!boss.SerializeToString(&archive)) {
// 				throw std::runtime_error("Failed to serialize with protobuf");
// 			}
// 		}
// 
// 		// De-serialization.
// 		{
// 			Rich::Boss boss;
// 			if (!boss.ParseFromString(archive)) {
// 				throw std::runtime_error("Failed to parse with protobuf");
// 			}
// 
// 			std::cout << boss.first_name() << " " << boss.last_name() << std::endl;
// 			for (int i = 0; i < boss.slave_size(); ++i)
// 			{
// 				auto slave = boss.slave(i);
// 				std::cout << "Slave number: " << slave.number() << std::endl;
// 			}
// 		}
		waitForTerminationRequest();

	} catch (const std::exception& e) {
		logger().error("Error occurred: %s", e.what());
	}
	return Application::EXIT_OK;
}

}