#ifndef REMOTE_SERVER
#define REMOTE_SERVER

namespace server {

class RemoteServer: public Poco::Util::ServerApplication {
public:
	void initialize(Poco::Util::Application& self) override;
	int main(const ArgVec& args) override;
};

}

POCO_SERVER_MAIN(server::RemoteServer)

#endif