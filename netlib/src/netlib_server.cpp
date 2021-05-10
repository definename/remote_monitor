#include "netlib_server.h"
#include "netlib_util.h"

namespace netlib
{

netlib_server::netlib_server(const unsigned short port)
	: netlib_core(2)
	, acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	, netlib_sender_(netlib_mgr_) {
	start();
}

netlib_server::~netlib_server() {
	try {
		stop();
	} catch (const std::exception& e) {
		NETLIB_ERR_FMT("Server destruction error: %s", WHAT_TO_STR(e));
	}
}

void netlib_server::start() {
	run();
	acceptor_.listen();
	start_accept();
}

void netlib_server::stop() {
	if (is_running()) {
		NETLIB_INF("Server is being stopped");
		netlib_core::stop();
		netlib_sender_.stop();
		netlib_mgr_.release();
	}
}

void netlib_server::start_accept() {
	netlib_session::pointer_t new_session = netlib_session::create(io_context_);
	acceptor_.async_accept(
		new_session->socket(),
		[new_session, this](const boost::system::error_code & ec) {
			try {
				NETLIB_CHECK_SYSTEM_ERROR(ec);
				set_socket_options(new_session->socket());
				NETLIB_INF("New connection accepted...");

				NETLIB_ASYNC_READ_UNTIL(netlib_server::handle_receive, new_session, io_strand_);
			}
			catch (const std::exception& e) {
				NETLIB_ERR_FMT("Accept handler error: %s", WHAT_TO_STR(e));
			}
			start_accept();
		});
}

void netlib_server::handle_receive(const netlib_session::pointer_t session, const boost::system::error_code ec, std::size_t size) {
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
		} else if (payload.has_connect()) {
			auto connect = payload.mutable_connect();
			session->set_session_id(boost::uuids::string_generator()(connect->session_id()));
			netlib_mgr_.add(session);
			connect->set_ready(true);
			NETLIB_INF_FMT("Session:%s has been added", connect->session_id());
			netlib_sender_.send(session->session_id(), payload);
			on_session_connected_(session->session_id());
		} else if (payload.has_data()) {
			on_session_received_(
				session->session_id(),
				reinterpret_cast<const netlib_message::buff_t::value_type*>(payload.data().c_str()),
				payload.data().size());
		} else {
			NETLIB_ERR("Invalid protocol");
		}

		session->buffer().consume(size);
		NETLIB_ASYNC_READ_UNTIL(netlib_server::handle_receive, session, io_strand_);
	}
	catch (const std::exception& e) {
		NETLIB_ERR_FMT("Receive handler error: %s", WHAT_TO_STR(e));
		netlib_mgr_.remove(session->session_id());
		on_session_disconnected_(session->session_id());
	}
}

}