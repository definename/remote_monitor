#ifndef NETLIB_SENDER
#define NETLIB_SENDER

#include "netlib_session.h"
#include "netlib_message.h"
#include "netlib_util.h"
#include "netlib_log.h"

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
	template <typename protobuf_t>
	void send(const netlib_session::sessionid_t& id, const protobuf_t& protobuf_msg);
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

template <typename protobuf_t>
void netlib_sender::send(const netlib_session::sessionid_t& id, const protobuf_t& protobuf_msg) {
	try {
		auto session = netlib_mgr_.find(id);
		netlib_message::pointer msg = std::make_shared<netlib_message>();
		msg->set_buff<protobuf_t>(protobuf_msg);
		msg->buff().insert(
			msg->buff().end(),
			std::begin(NETLIB_DELIMITER_VALUE),
			std::end(NETLIB_DELIMITER_VALUE));

		{
			std::unique_lock<std::mutex> l(mutex_);
			sending_queue_.push({ msg, session });
			cv_.notify_all();
		}
	}
	catch (const std::exception & e) {
		NETLIB_ERR_FMT("Failed to send protocol message:%s", WHAT_TO_STR(e));
	}
}

}

#endif