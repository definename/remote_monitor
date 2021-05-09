#ifndef NETLIB_CLIENT
#define NETLIB_CLIENT

#include "netlib_core.h"

namespace netlib {

class netlib_client : public netlib_core {
	using tcp_resolver = boost::asio::ip::tcp::resolver;

	// Public interface
public:
	// Constructor
	netlib_client(const boost::uuids::uuid& id);
	void connect(const boost::asio::ip::tcp::endpoint ep, const boost::uuids::uuid& id);

	// Private members
private:
	// Client id
	boost::uuids::uuid id_;
	tcp_resolver resolver_;
};

}

#endif