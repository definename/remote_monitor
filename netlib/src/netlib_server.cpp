#include "netlib_server.h"

namespace netlib
{

netlib_server::netlib_server()
	: netlib_core(2)
	, acceptor_(io_context_) {
	run();
}

netlib_server::~netlib_server() {
	try {
		stop();
	} catch (const std::exception& e) {
		NETLIB_ERR_FMT("Server destruction error: %s", e.what());
	}
}

}