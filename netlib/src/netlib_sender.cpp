#include "netlib_sender.h"
#include "netlib_message.h"
#include "netlib_mgr.h"
#include "netlib_util.h"

namespace netlib {

netlib_sender::netlib_sender(netlib_mgr& mgr)
	: netlib_mgr_(mgr)
	, sending_thread_()
	, is_running_(false) {
	sending_thread_ = std::thread(std::bind(&netlib_sender::handle_send, this));
}

void netlib_sender::handle_send() {
	is_running_ = true;
	while (is_running_) {
		std::unique_lock<std::mutex> l(mutex_);
		auto pred = [this]() { return !sending_queue_.empty(); };
		if (cv_.wait_for(l, std::chrono::milliseconds(500), pred)) {
			while (!sending_queue_.empty() && is_running_) {
				try {
					queue_t::value_type item = sending_queue_.front();
					sending_queue_.pop();

					boost::asio::write(
						item.session_->socket(),
						boost::asio::buffer(item.msg_->buff().data(), item.msg_->buff().size()));
				} catch (const std::exception& e) {
					NETLIB_ERR_FMT("Failed to send message:%s", WHAT_TO_STR(e));
				}
			}
		}
	}	
}

void netlib_sender::send(const netlib_session::sessionid_t& id, const protocol::Payload& payload) {
	try {
		auto session = netlib_mgr_.find(id);
		netlib_message::pointer msg = std::make_shared<netlib_message>();
		msg->set_buff<decltype(payload)>(payload);
		send(id, msg);
	}
	catch (const std::exception & e) {
		NETLIB_ERR_FMT("Failed to send protocol message:%s", WHAT_TO_STR(e));
	}
}

void netlib_sender::send(const netlib_session::sessionid_t& id, netlib_message::pointer& msg) {
	try {
		netlib_session::pointer_t session = netlib_mgr_.find(id);
		msg->buff().insert(
			msg->buff().end(),
			std::begin(NETLIB_DELIMITER_VALUE),
			std::end(NETLIB_DELIMITER_VALUE));

		{
			std::unique_lock<std::mutex> l(mutex_);
			sending_queue_.push({ msg, session });
			cv_.notify_all();
			NETLIB_INF("push to queue");
		}
	} catch (const std::exception& e) {
		NETLIB_ERR_FMT("Failed to send message:%s", WHAT_TO_STR(e));
	}
}

void netlib_sender::stop() {
	{
		is_running_ = false;
		std::unique_lock<std::mutex> l(mutex_);
		sending_queue_.swap(queue_t());
	}
	sending_thread_.join();
}

}