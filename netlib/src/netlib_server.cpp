#include "netlib_server.h"
#include "netlib_session.h"

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
			}
			catch (const std::exception& e) {
				NETLIB_ERR_FMT("Accept handling erro: %s", e.what());
			}
			start_accept();
		});
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

}