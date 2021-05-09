#include "netlib_message.h"

namespace netlib
{

message::~message() {
	buff_t().swap(buffer_);
}

message::buff_t& message::buff() {
	return buffer_;
}

message::buff_t::value_type* message::data() {
	return buffer_.data();
}

const size_t message::size() const {
	return buffer_.size();
}

}