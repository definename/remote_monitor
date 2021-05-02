#ifndef NETLIB_LOG
#define NETLIB_LOG

#include <Poco/Logger.h>
#include <Poco/Message.h>

#define NETLIB_POCO_LOGGER_NAME "ApplicationStartup"
#define NETLIB_POCO_LOGGER_SOURCE "netlib"

#define NETLIB_FTL(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_FATAL))			\

#define NETLIB_CRT(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_CRITICAL))		\

#define NETLIB_ERR(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_ERROR))			\

#define NETLIB_WRN(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_WARNING))		\

#define NETLIB_NTC(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_NOTICE))		\

#define NETLIB_INF(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_INFORMATION))	\

#define NETLIB_DBG(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_DEBUG))			\

#define NETLIB_TRC(msg)														\
Poco::Logger::get(NETLIB_POCO_LOGGER_NAME).log(Poco::Message(				\
		NETLIB_POCO_LOGGER_SOURCE, msg, Poco::Message::PRIO_TRACE))			\

#endif