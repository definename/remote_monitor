#ifndef REMOTE_SERVER
#define REMOTE_SERVER

namespace server {

class remote_client: public Poco::Util::ServerApplication {
public:
	remote_client();
	int main(const ArgVec& args) override;

private:
	std::shared_ptr<netlib::netlib_client> netlib_client_;
};

}

POCO_SERVER_MAIN(server::remote_client)

#endif