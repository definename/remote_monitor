#include "netlib_client.h"
#include "netlib_session.h"
#include "netlib_util.h"

namespace netlib {

netlib_client::netlib_client(const boost::uuids::uuid& id)
	: netlib_core(2)
	, id_(id)
	, resolver_(io_context_) {
	run();
}

void netlib_client::connect(const boost::asio::ip::tcp::endpoint ep, const boost::uuids::uuid& id) {
	if (!is_running()) {
		NETLIB_ERR("Client io context is not running");
		return;
	}

	NETLIB_INF_FMT("Ñonnect to: %s:%s", ep.address().to_string(), ep.port());
	tcp_resolver::query query(boost::asio::ip::tcp::v4(), ep.address().to_string(), std::to_string(ep.port()));
	netlib_session::pointer_t new_session = netlib_session::create(io_context_, id_generator_());
	netlib_session::tcp_socket_t& socket = new_session->socket();
	socket.open(boost::asio::ip::tcp::v4());
	set_socket_options(socket);
	resolver_.async_resolve(
		ep.address().to_string(),
		std::to_string(ep.port()),
		boost::asio::bind_executor(io_strand_, [new_session, this](
			const boost::system::error_code& ec, tcp_resolver::results_type results) {
				try {
					NETLIB_CHECK_SYSTEM_ERROR(ec);
					NETLIB_INF("Endpoint has been resolved");
					boost::asio::async_connect(
						new_session->socket(),
						results,
						boost::asio::bind_executor(io_strand_, [new_session, this](
							const boost::system::error_code& ec, const boost::asio::ip::tcp::endpoint& ep) {
								try {
									NETLIB_CHECK_SYSTEM_ERROR(ec);
									NETLIB_INF("Client has been connected");
								} catch (const std::exception& e) {
									NETLIB_ERR_FMT("Failed to connect to given endpoint:%s", e.what());
								}
							}));
				} catch (const std::exception& e) {
					NETLIB_ERR_FMT("Failed to resolve given endpoint:%s", e.what());
				}
			}));

}

}