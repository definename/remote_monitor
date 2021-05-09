#ifndef NETLIB_SESSION
#define NETLIB_SESSION

namespace netlib {

class netlib_session : public boost::enable_shared_from_this<netlib_session> {
	// Public types
public:
	using pointer = boost::shared_ptr<netlib_session>;
	using tcp_socket = boost::asio::ip::tcp::socket;
	using buff = boost::asio::streambuf;

	// Public interface
public:
	~netlib_session() = default;
	netlib_session(const netlib_session&) = delete;
	netlib_session(netlib_session&&) = delete;
	netlib_session& operator=(const netlib_session&) = delete;
	netlib_session& operator=(netlib_session&&) = delete;

	static pointer create(boost::asio::io_context& io_context);
	void stop();
	tcp_socket& socket();
	buff& buffer();

	// protected interface
protected:
	netlib_session(boost::asio::io_context& io_context);

	// public members
private:
	tcp_socket socket_;
	buff buffer_;
};

}

#endif