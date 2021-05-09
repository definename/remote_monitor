#ifndef NETLIB_MESSAGE
#define NETLIB_MESSAGE

namespace netlib {

class message
{
	using buff_t = std::vector<unsigned int>;

	// Public interface
public:
	//! Constructor.
	message() = default;
	//! Destructor.
	~message();

	//! Returns a direct pointer to the memory array used internally by the message.
	buff_t::value_type* data();
	//! Returns a reference to the array used internally by the message.
	buff_t& buff();
	//! Returns message size.
	const size_t size() const;
	//! Sets message buffer.
	template <typename protobuf_t>
	void set_buff(protobuf_t msg);

	// Private members.
private:
	//! Buffer.
	buff_t buffer_;
};

template <typename protobuf_t>
void message::set_buff(protobuf_t msg) {
	buffer_.resize(msg->ByteSize());
	if (!msg->SerializeToArray(buffer_.data(), msg->ByteSize())) {
		throw std::runtime_error("Failed to serialize with protobuf");
	}
}

}



#endif