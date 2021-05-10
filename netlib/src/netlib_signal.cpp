#include "netlib_signal.h"

namespace netlib {

boost::signals2::connection netlib_signal::do_on_session_connected(const on_session_connected::slot_type& signal) {
	return on_session_connected_.connect(signal);
}

boost::signals2::connection netlib_signal::do_on_session_disconnected(const on_session_disconnected::slot_type& signal) {
	return on_session_disconnected_.connect(signal);
}

boost::signals2::connection netlib_signal::do_on_session_received(const on_session_received::slot_type& signal) {
	return on_session_received_.connect(signal);
}

}