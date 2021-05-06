#ifndef NETLIB_SERVER
#define NETLIB_SERVER

#include "netlib_core.h"

namespace netlib
{

class netlib_server: public netlib_core
{
	// Construction and destruction.
public:
	//! Constructor.
	netlib_server();
	//! Destructor.
	~netlib_server();

	// Private members.
private:
	boost::asio::ip::tcp::acceptor acceptor_;
};

}

#endif