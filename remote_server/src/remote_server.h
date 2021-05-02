#ifndef REMOTE_SERVER
#define REMOTE_SERVER

namespace server {

class DoIt {
public:
	void Do();
};

class RemoteServer: public Poco::Util::ServerApplication {
public:
	void initialize(Poco::Util::Application& self) override;
	int main(const ArgVec& args) override;

	DoIt do_;
};

}

POCO_SERVER_MAIN(server::RemoteServer)

#endif