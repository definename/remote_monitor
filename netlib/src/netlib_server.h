#ifndef NETLIB_SERVER
#define NETLIB_SERVER

#include "netlib_core.h"
#include "netlib_mgr.h"
#include "netlib_message.h"
#include "netlib_session.h"
#include "netlib_sender.h"
#include "netlib_signal.h"

namespace netlib
{

class netlib_server
	: public netlib_core
	, public netlib_signal
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
	// Send message to session with given id
	template <typename protobuf_t>
	void send(const netlib_session::sessionid_t& id, const protobuf_t& protobuf_msg);

private:
	// Accept new connection
	void start_accept();
	// Receiving handler
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

template <typename protobuf_t>
void netlib_server::send(const netlib_session::sessionid_t& id, const protobuf_t& protobuf_msg) {
	std::string data_msg;
	if (!protobuf_msg.SerializeToString(&data_msg)) {
		throw std::runtime_error("Failed to serialize with protobuf");
	}
	protocol::Payload payload;
	payload.set_data(data_msg);
	netlib_sender_.send(id, payload);
}

}

#endif