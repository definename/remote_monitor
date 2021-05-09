#ifndef NETLIB_CORE
#define NETLIB_CORE

namespace netlib
{

class netlib_core {
	// Construction and destruction.
public:
	//! Constructor.
	explicit netlib_core(const unsigned int workers = 1);
	//! Destructor.
	virtual ~netlib_core() = default;

	// Protected interface.
protected:
	//! Starts.
	void run();
	//! Stops.
	virtual void stop();
	//! Is running
	bool is_running() const;

	// Protected data members.
protected:
	//! IO context
	boost::asio::io_context io_context_;
	// IO strand
	boost::asio::strand<boost::asio::io_context::executor_type> io_strand_;
	//! IO work
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> io_work_;
	//! IO threads
	std::vector<std::thread> threads_;
	//! Workers count
	const unsigned int workers_;
	//! Is running
	bool is_running_;
};

}

#endif