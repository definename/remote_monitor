#include "netlib_server.h"

#define NETLIB_DELIMITER_VALUE protocol::Delimiter().default_instance().value()
#define NETLIB_DELIMITER_SIZE NETLIB_DELIMITER_VALUE.size()

namespace netlib
{

netlib_server::netlib_server(const unsigned short port)
	: netlib_core(2)
	, acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	start();
}

netlib_server::~netlib_server() {
	try {
		stop();
	} catch (const std::exception& e) {
		NETLIB_ERR_FMT("Server destruction error: %s", e.what());
	}
}

void netlib_server::start() {
	run();
	acceptor_.listen();
	start_accept();
}

void netlib_server::stop() {
	if (is_running()) {
		NETLIB_INF("Netlib server is being stopped");
		netlib_core::stop();
	}
}

void netlib_server::start_accept() {
	netlib_session::pointer new_session = netlib_session::create(io_context_);
	acceptor_.async_accept(
		new_session->socket(),
		[new_session, this](const boost::system::error_code & ec) {
			try {
				if (ec) {
					throw std::runtime_error(ec.message());
				}

				boost::system::error_code ecode;
				new_session->socket().set_option(boost::asio::ip::tcp::no_delay(true), ecode);
				if (ecode) {
					throw std::runtime_error(ecode.message());
				}
				new_session->socket().set_option(boost::asio::socket_base::keep_alive(true), ecode);
				if (ecode) {
					throw std::runtime_error(ecode.message());
				}
				NETLIB_INF("New connection accepted...");

				boost::asio::async_read_until(
					new_session->socket(),
					new_session->buffer(),
					NETLIB_DELIMITER_VALUE,
					boost::asio::bind_executor(io_strand_, boost::bind(&netlib_server::handle_receive,
						this, new_session, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
			}
			catch (const std::exception& e) {
				NETLIB_ERR_FMT("Accept handler error: %s", e.what());
			}
			start_accept();
		});
}

void netlib_server::handle_receive(const netlib_session::pointer session, const boost::system::error_code ec, std::size_t size) {
	try {

		if (ec) {
			throw std::runtime_error(ec.message());
		}

		protocol::Payload payload;
		if (!payload.ParseFromArray(
			boost::asio::buffer_cast<const unsigned char*>(session->buffer().data()),
			static_cast<int>(size - NETLIB_DELIMITER_SIZE))) {
			throw std::runtime_error("Unable to parse protocol buffer");
		}

		if (payload.has_heartbeat()) {
			NETLIB_INF("heartbeat received");
		}

		session->buffer().consume(size);
		boost::asio::async_read_until(
			session->socket(),
			session->buffer(),
			NETLIB_DELIMITER_VALUE,
			boost::asio::bind_executor(io_strand_, boost::bind(&netlib_server::handle_receive,
				this, session, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
	}
	catch (const std::exception& e) {
		NETLIB_ERR_FMT("Receive handler error: %s", e.what());
	}
}

}