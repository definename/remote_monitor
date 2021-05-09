#ifndef NETLIB_MGR
#define NETLIB_MGR

#include "netlib_session.h"

namespace netlib {

class netlib_mgr
{
	using session_map_t = boost::unordered_map<boost::uuids::uuid, netlib_session::pointer_t>;

	// Public interface.
public:
	//! Constructor.
	netlib_mgr() = default;
	//! Destructor.
	~netlib_mgr();

	//! Adds session.
	bool add(netlib_session::pointer_t session);
	//! Finds session with given id.
	netlib_session::pointer_t find(const netlib_mgr::session_map_t::key_type& id);
	//! Removes session with given id
	void remove(const netlib_mgr::session_map_t::key_type& id);
	//! Release
	void release();

	//
	// Private data members.
	//
private:
	//! Synchronization mutex.
	std::mutex mutex_;
	//! Sessions' map.
	session_map_t session_list_;
};

}

#endif