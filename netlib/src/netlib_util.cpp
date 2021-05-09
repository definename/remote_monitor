#include "netlib_util.h"

void set_socket_options(boost::asio::ip::tcp::socket& socket) {
	boost::system::error_code ecode;
	socket.set_option(boost::asio::ip::tcp::no_delay(true), ecode);
	if (ecode) {
		throw std::runtime_error(ecode.message());
	}
	socket.set_option(boost::asio::socket_base::keep_alive(true), ecode);
	if (ecode) {
		throw std::runtime_error(ecode.message());
	}
}