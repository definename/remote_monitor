#ifndef NETLIB_LOG
#define NETLIB_LOG

#include <Poco/Logger.h>
#include <Poco/Message.h>

#define NETLIB_POCO_LOGGER_NAME "Application"
#define NETLIB_POCO_LOGGER_SOURCE "netlib"

#define NETLIB_FTL(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_FATAL))			\

#define NETLIB_FTL_FMT(fmt, ...)											\
		NETLIB_FTL(Poco::format((fmt), (##__VA_ARGS__)))					\

#define NETLIB_CRT(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_CRITICAL))		\

#define NETLIB_CRT_FMT(fmt, ...)											\
		NETLIB_CRT(Poco::format((fmt), (##__VA_ARGS__)))					\

#define NETLIB_ERR(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_ERROR))			\

#define NETLIB_ERR_FMT(fmt, ...)											\
		NETLIB_ERR(Poco::format((fmt), (##__VA_ARGS__)))					\

#define NETLIB_WRN(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_WARNING))		\

#define NETLIB_WRN_FMT(fmt, ...)											\
		NETLIB_WRN(Poco::format((fmt), (##__VA_ARGS__)))					\

#define NETLIB_NTC(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_NOTICE))		\

#define NETLIB_NTC_FMT(fmt, ...)											\
		NETLIB_NTC(Poco::format((fmt), (##__VA_ARGS__)))					\

#define NETLIB_INF(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_INFORMATION))	\

#define NETLIB_INF_FMT(fmt, ...)											\
		NETLIB_INF(Poco::format((fmt), (##__VA_ARGS__)))					\

#define NETLIB_DBG(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_DEBUG))			\

#define NETLIB_DBG_FMT(fmt, ...)											\
		NETLIB_DBG(Poco::format((fmt), (##__VA_ARGS__)))					\

#define NETLIB_TRC(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_TRACE))			\

#define NETLIB_TRC_FMT(fmt, ...)											\
		NETLIB_TRC(Poco::format((fmt), (##__VA_ARGS__)))					\

#endif