#include "netlib_session.h"

namespace netlib {

netlib_session::netlib_session(boost::asio::io_context& io_context)
	: socket_(io_context) {
}

netlib_session::pointer netlib_session::create(boost::asio::io_context& io_context) {
	return netlib_session::pointer(new netlib_session(io_context));
}

void netlib_session::stop() {
	if (socket_.is_open()) {
		boost::system::error_code ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		socket_.close(ec);
	}
}

netlib_session::tcp_socket& netlib_session::socket() {
	return socket_;
}

netlib_session::buff& netlib_session::buffer() {
	return buffer_;
}

}