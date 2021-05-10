#ifndef NETLIB_CLIENT
#define NETLIB_CLIENT

#include "netlib_core.h"
#include "netlib_mgr.h"
#include "netlib_sender.h"

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
	// Stop client
	void stop() override;
	// Send message to session with given id
	template <typename protobuf_t>
	void send(const netlib_session::sessionid_t& id, const protobuf_t& protobuf_msg) const;

private:
	// Receiving handler
	void handle_receive(const netlib_session::pointer_t session, const boost::system::error_code ec, std::size_t size);

	// Private members
private:
	// Resolver
	tcp_resolver resolver_;
	// Session manager
	netlib_mgr netlib_mgr_;
	// Sender
	netlib_sender netlib_sender_;
};

template <typename protobuf_t>
void netlib_client::send(const netlib_session::sessionid_t& id, const protobuf_t& protobuf_msg) const {
	netlib_sender_.send(id, protobuf_msg);
}

}

#endif