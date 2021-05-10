#ifndef NETLIB_UTIL
#define NETLIB_UTIL

#include "Protocol.pb.h"

#define NETLIB_DELIMITER_VALUE protocol::Delimiter().default_instance().value()
#define NETLIB_DELIMITER_SIZE NETLIB_DELIMITER_VALUE.size()

#define NETLIB_CHECK_SYSTEM_ERROR(ec)				\
do													\
{													\
	if (ec) {										\
		throw std::runtime_error(ec.message());		\
	}												\
} while (0)											\


#define NETLIB_ASYNC_READ_UNTIL(handler, session, io_strand)														\
boost::asio::async_read_until(																						\
	session##->socket(),																							\
	session##->buffer(),																							\
	NETLIB_DELIMITER_VALUE,																							\
	boost::asio::bind_executor(##io_strand, boost::bind(&##handler,													\
		this, session, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)))			\


void set_socket_options(boost::asio::ip::tcp::socket& socket);

#endif