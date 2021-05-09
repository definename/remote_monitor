#include "netlib_message.h"

namespace netlib
{

netlib_message::~netlib_message() {
	buff_t().swap(buffer_);
}

netlib_message::buff_t& netlib_message::buff() {
	return buffer_;
}

netlib_message::buff_t::value_type* netlib_message::data() {
	return buffer_.data();
}

const size_t netlib_message::size() const {
	return buffer_.size();
}

}