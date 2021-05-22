#ifndef MONITOR_LOG
#define MONITOR_LOG

#define MONITOR_POCO_LOGGER_NAME "Application"
#define MONITOR_POCO_LOGGER_SOURCE "Monitor"

#define LOG_INF(msg)														\
Poco::Logger::get(MONITOR_POCO_LOGGER_NAME).log(Poco::Message(				\
		MONITOR_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_INFORMATION))	\

#define LOG_INF_FMT(fmt, ...)												\
		LOG_INF(Poco::format(fmt, ##__VA_ARGS__))							\

#define LOG_ERR(msg)														\
Poco::Logger::get(MONITOR_POCO_LOGGER_NAME).log(Poco::Message(				\
		MONITOR_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_ERROR))		\

#define LOG_ERR_FMT(fmt, ...)												\
		LOG_ERR(Poco::format(fmt, ##__VA_ARGS__))							\

#define LOG_WHAT2STR(e) std::string(e.what())

#endif