#ifndef REMOTE_SERVER
#define REMOTE_SERVER

namespace screen {
// Forward declaration
class viewport;

}

namespace daemon {

class remote_server: public Poco::Util::ServerApplication {
public:
	remote_server();
	int main(const ArgVec& args) override;

private:
	void send(const netlib::netlib_session::sessionid_t&, const netlib::viewport::Frame&);

private:
	std::shared_ptr<netlib::netlib_server> netlib_server_;
	std::shared_ptr<screen::viewport> viewport_;
};

}

POCO_SERVER_MAIN(daemon::remote_server)

#endif