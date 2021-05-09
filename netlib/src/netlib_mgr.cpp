#include "netlib_mgr.h"

namespace netlib {

netlib_mgr::~netlib_mgr() {
	try {
		release();
	} catch (const std::exception & e) {
		NETLIB_ERR_FMT("Session manager error:%s", WHAT_TO_STR(e));
	}
}

bool netlib_mgr::add(netlib_session::pointer_t session) {
	std::unique_lock<std::mutex> l(mutex_);
	return session_list_.insert(std::make_pair(session->session_id(), session)).second;
}

netlib_session::pointer_t netlib_mgr::find(const netlib_mgr::session_map_t::key_type& id)
{
	std::unique_lock<std::mutex> l(mutex_);
	auto ret = session_list_.find(id);
	if (ret == session_list_.end()) {
		throw std::runtime_error("Unable to find session with given id: " + boost::uuids::to_string(id));
	}
	return ret->second;
}

void netlib_mgr::remove(const netlib_mgr::session_map_t::key_type& id)
{
	std::unique_lock<std::mutex> l(mutex_);
	auto ret = session_list_.find(id);
	if (ret == session_list_.end()) {
		NETLIB_ERR_FMT("Unable to find session with given id:%s", boost::uuids::to_string(id));
		return;
	}

	if (ret->second) {
		ret->second->stop();
		ret->second.reset();
	}
	session_list_.erase(id);
}

void netlib_mgr::release() {
	std::unique_lock<std::mutex> l(mutex_);
	for (netlib_mgr::session_map_t::value_type& val : session_list_) {
		val.second->stop();
		val.second.reset();
	}
	session_list_.clear();
}

}