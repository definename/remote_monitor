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
	netlib_server(const unsigned short port);
	//! Destructor.
	~netlib_server();

public:
	// Starts server
	void start();
	// Stops server
	void stop() override;

private:
	// Accept new connection
	void start_accept();

	// Private members.
private:
	boost::asio::ip::tcp::acceptor acceptor_;
};

}

#endif