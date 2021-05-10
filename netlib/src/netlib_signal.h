#ifndef NETLIB_SIGNAL
#define NETLIB_SIGNAL

#include "netlib_session.h"
#include "netlib_message.h"

namespace netlib {

class netlib_signal {
	// Public types
public:
	using on_session_connected = boost::signals2::signal<
		void(const netlib_session::sessionid_t&)>;
	using on_session_disconnected = boost::signals2::signal<
		void(const netlib_session::sessionid_t& id)>;
	using on_session_received = boost::signals2::signal<
		void(const netlib_session::sessionid_t& id, const netlib_message::buff_t::value_type* data, const std::size_t bytes)>;

	// Public interface
public:
	virtual ~netlib_signal() = default;

	boost::signals2::connection do_on_session_connected(const on_session_connected::slot_type& signal);
	boost::signals2::connection do_on_session_disconnected(const on_session_disconnected::slot_type& signal);
	boost::signals2::connection do_on_session_received(const on_session_received::slot_type& signal);

	// Protected members
protected:
	on_session_connected on_session_connected_;
	on_session_disconnected on_session_disconnected_;
	on_session_received on_session_received_;
};

}

#endif