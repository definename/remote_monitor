#include "netlib_client.h"
#include "netlib_session.h"
#include "netlib_util.h"

namespace netlib {

netlib_client::netlib_client()
	: netlib_core(2)
	, resolver_(io_context_)
	, netlib_sender_(netlib_mgr_) {
	run();
}

netlib_client::~netlib_client() {
	stop();
}

void netlib_client::stop() {
	if (is_running()) {
		NETLIB_INF("Client is being stopped");
		netlib_core::stop();
		netlib_sender_.stop();
		netlib_mgr_.release();
	}
}

void netlib_client::connect(const std::string& addr, const unsigned port) {
	if (!is_running()) {
		NETLIB_ERR("Client io context is not running");
		return;
	}
	NETLIB_INF_FMT("Connecting to %s:%u", addr, port);
	netlib_session::pointer_t new_session = netlib_session::create(io_context_, id_generator_());
	netlib_session::tcp_socket_t& socket = new_session->socket();
	socket.open(boost::asio::ip::tcp::v4());
	set_socket_options(socket);
	resolver_.async_resolve(
		addr,
		std::to_string(port),
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

									if (!netlib_mgr_.add(new_session)) {
										throw std::runtime_error("Failed to add session with given id");
									}

									NETLIB_ASYNC_READ_UNTIL(netlib_client::handle_receive, new_session, io_strand_);

									protocol::Payload payload;
									protocol::Connect* connect = payload.mutable_connect();
									connect->set_session_id(boost::uuids::to_string(new_session->session_id()));
									netlib_sender_.send(new_session->session_id(), payload);

								} catch (const std::exception& e) {
									NETLIB_ERR_FMT("Failed to connect to given endpoint:%s", WHAT_TO_STR(e));
								}
							}));
				} catch (const std::exception& e) {
					NETLIB_ERR_FMT("Failed to resolve given endpoint:%s", WHAT_TO_STR(e));
				}
			}));
}

void netlib_client::handle_receive(const netlib_session::pointer_t session, const boost::system::error_code ec, std::size_t size) {
	try {
		NETLIB_CHECK_SYSTEM_ERROR(ec);
		protocol::Payload payload;
		if (!payload.ParseFromArray(
			boost::asio::buffer_cast<const unsigned char*>(session->buffer().data()),
			static_cast<int>(size - NETLIB_DELIMITER_SIZE))) {
			throw std::runtime_error("Unable to parse protocol buffer");
		}

		if (payload.has_heartbeat()) {
			NETLIB_INF("has_heartbeat");
		}
		else if (payload.has_connect()) {
			auto connect = payload.mutable_connect();
			if (connect->has_ready() && connect->ready()) {
				NETLIB_INF_FMT("Session has been initialized:%s", connect->session_id());
			}
		}
		else if (payload.has_data()) {
			NETLIB_INF("has_data");
		}
		else {
			NETLIB_ERR("Invalid protocol");
		}

		session->buffer().consume(size);
		NETLIB_ASYNC_READ_UNTIL(netlib_client::handle_receive, session, io_strand_);
	}
	catch (const std::exception & e) {
		NETLIB_ERR_FMT("Receive handler error: %s", WHAT_TO_STR(e));
	}
}

}