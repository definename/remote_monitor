#include "netlib_session.h"

namespace netlib {

netlib_session::netlib_session(boost::asio::io_context& io_context, const sessionid_t& id /*= boost::uuids::nil_uuid()*/)
	: socket_(io_context)
	, session_id_(id) {
}

netlib_session::~netlib_session() {
	stop();
}

netlib_session::pointer_t netlib_session::create(
	boost::asio::io_context& io_context,
	const sessionid_t& id /*= boost::uuids::nil_uuid()*/) {
	return netlib_session::pointer_t(new netlib_session(io_context, id));
}

void netlib_session::stop() {
	if (socket_.is_open()) {
		NETLIB_INF_FMT("Session %s is being stopped", boost::uuids::to_string(session_id()));
		boost::system::error_code ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		socket_.close(ec);
	}
}

netlib_session::tcp_socket_t& netlib_session::socket() {
	return socket_;
}

netlib_session::buff_t& netlib_session::buffer() {
	return buffer_;
}

netlib_session::sessionid_t netlib_session::session_id() const {
	return session_id_;
}

}