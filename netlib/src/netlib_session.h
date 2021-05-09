#ifndef NETLIB_SESSION
#define NETLIB_SESSION

namespace netlib {

class netlib_session : public boost::enable_shared_from_this<netlib_session> {
	// Public types
public:
	using pointer_t = boost::shared_ptr<netlib_session>;
	using tcp_socket_t = boost::asio::ip::tcp::socket;
	using buff_t = boost::asio::streambuf;
	using sessionid_t = boost::uuids::uuid;

	// Public interface
public:
	~netlib_session() = default;
	netlib_session(const netlib_session&) = delete;
	netlib_session(netlib_session&&) = delete;
	netlib_session& operator=(const netlib_session&) = delete;
	netlib_session& operator=(netlib_session&&) = delete;

	static pointer_t create(boost::asio::io_context& io_context, const sessionid_t& id = boost::uuids::nil_uuid());
	void stop();
	tcp_socket_t& socket();
	buff_t& buffer();
	sessionid_t session_id() const;

	// protected interface
protected:
	netlib_session(boost::asio::io_context& io_context, const sessionid_t& id = boost::uuids::nil_uuid());

	// public members
private:
	tcp_socket_t socket_;
	buff_t buffer_;
	sessionid_t session_id_;
};

}

#endif