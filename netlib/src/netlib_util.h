#ifndef NETLIB_UTIL
#define NETLIB_UTIL

#define NETLIB_CHECK_SYSTEM_ERROR(ec)				\
do													\
{													\
	if (ec) {										\
		throw std::runtime_error(ec.message());		\
	}												\
} while (0)											\

void set_socket_options(boost::asio::ip::tcp::socket& socket);

#endif