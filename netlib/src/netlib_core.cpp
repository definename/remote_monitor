#include "netlib_core.h"

namespace netlib
{

netlib_core::netlib_core(const unsigned int workers /*= 1*/)
	: io_work_(boost::asio::make_work_guard(io_context_))
	, io_strand_(boost::asio::make_strand(io_context_))
	, is_running_(false)
	, workers_{workers} {
}

bool netlib_core::is_running() const {
	return is_running_;
}

void netlib_core::run() {
	if (!is_running_) {
		for (unsigned int i = 0; i != workers_; ++i) {
			threads_.emplace_back(std::bind(static_cast<size_t(boost::asio::io_context::*)()>(&boost::asio::io_context::run), &io_context_));
		}
		is_running_ = true;
	}
}

void netlib_core::stop() {
	if (is_running_) {
		is_running_ = false;
		io_context_.stop();
		std::for_each(threads_.begin(), threads_.end(), [](std::thread& t) { t.join(); });
		threads_.clear();
	}
}

}