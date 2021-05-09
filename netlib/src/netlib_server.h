#ifndef NETLIB_SERVER
#define NETLIB_SERVER

#include "netlib_core.h"
#include "netlib_session.h"
#include "netlib_mgr.h"
#include "netlib_sender.h"

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
	void handle_receive(const netlib_session::pointer_t session, const boost::system::error_code ec, std::size_t size);

	// Private members.
private:
	// Acceptor
	boost::asio::ip::tcp::acceptor acceptor_;
	// Session manager
	netlib_mgr netlib_mgr_;
	// Sender
	netlib_sender netlib_sender_;
};

}

#endif