#ifndef NETLIB_SERVER
#define NETLIB_SERVER

#include "netlib_core.h"
#include "netlib_session.h"

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
	//
	void handle_receive(const netlib_session::pointer session, const boost::system::error_code ec, std::size_t size);

	// Private members.
private:
	boost::asio::ip::tcp::acceptor acceptor_;
};

}

#endif