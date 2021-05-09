#ifndef NETLIB_CLIENT
#define NETLIB_CLIENT

#include "netlib_core.h"

namespace netlib {

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

	// Private members
private:
	// Client id
	tcp_resolver resolver_;
};

}

#endif