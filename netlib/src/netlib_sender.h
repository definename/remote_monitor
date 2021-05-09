#ifndef NETLIB_SENDER
#define NETLIB_SENDER

#include "netlib_session.h"
#include "netlib_message.h"

namespace protocol {
	class Payload;
}

namespace netlib {

class netlib_mgr;

class netlib_sender {
	// Public types
public:
	struct queue_item_t {
		netlib_message::pointer msg_;
		netlib_session::pointer_t session_;
	};
	using queue_t = std::queue<queue_item_t>;

	// Public interface
public:
	// Constructor
	netlib_sender(netlib_mgr& mgr);
	// Destructor
	~netlib_sender() = default;

	// Interface to send protocol message
	void send(const netlib_session::sessionid_t& id, const protocol::Payload& payload);
	// Interface to send message
	void send(const netlib_session::sessionid_t& id, netlib_message::pointer& msg);
	// Stops
	void stop();

	// Private interface
private:
	// Send handler
	void handle_send();

private:
	// Mgr reference
	netlib_mgr& netlib_mgr_;
	// Sending thread
	std::thread sending_thread_;
	// Sending queue
	queue_t sending_queue_;
	// Synchronization mutex
	std::mutex mutex_;
	// Condition
	std::condition_variable_any cv_;
	// Is running flag
	bool is_running_;
};

}

#endif