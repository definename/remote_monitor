#ifndef REMOTE_SERVER
#define REMOTE_SERVER

namespace server {

class remote_server: public Poco::Util::ServerApplication {
public:
	remote_server();
	int main(const ArgVec& args) override;

private:
	std::shared_ptr<netlib::netlib_server> netlib_server_;
};

}

POCO_SERVER_MAIN(server::remote_server)

#endif