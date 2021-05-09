#ifndef NETLIB_CLIENT
#define NETLIB_CLIENT

#include "netlib_core.h"
#include "netlib_mgr.h"

namespace netlib {

class netlib_mgr;

class netlib_client : public netlib_core {
	using tcp_resolver = boost::asio::ip::tcp::resolver;

	// Public interface
public:
	// Constructor
	netlib_client();
	// Destructor
	~netlib_client();

	// Interface to connect to given endpoint
	void connect(const std::string& addr, const unsigned port);
	// Stop client
	void stop() override;

	// Private members
private:
	// Resolver
	tcp_resolver resolver_;
	// Session manager
	netlib_mgr netlib_mgr_;
};

}

#endif