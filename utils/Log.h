/**
  ******************************************************************************
  * @file           : Log.h
  * @author         : xy
  * @brief          : 封装 log4cpp 日志库
  * @attention      : 当日志系统有多个日志目的地时，每一个目的地 Appender 都需要设置一个布局 Layout（一对一关系）
  * @date           : 2024/10/28
  ******************************************************************************
  */

#ifndef WEBDESK_UTILS_LOG_H_
#define WEBDESK_UTILS_LOG_H_

#include <iostream>
#include <memory>
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <string>
#include <sstream>

#include "Singleton.h"

enum LOGLEVEL { EMERG, FATAL, ALERT, CRIT, ERROR, WARN, NOTICE, INFO, DEBUG };

namespace WebDesk {

class Log : public Singleton<Log>, public std::enable_shared_from_this<Log> {
  friend class Singleton<Log>;
 public:
  void setLogLevel(log4cpp::Priority::Value priority);
  std::string int2string(int lineNumber);
  void fatal(const char *msg);
  void error(const char *msg);
  void warn(const char *msg);
  void info(const char *msg);
  void debug(const char *msg);
  ~Log();
 private:
  Log();
 private:
  log4cpp::Category &m_rootCategory;
};

#define catMsg(msg) std::string(msg).append(" {fileName:")\
    .append(__FILE__).append(" functionName:")\
    .append(__func__).append(" lineNumber:")\
    .append(Log::Singleton::getInstance()->int2string(__LINE__)).append("}").c_str()

#define logSetLogLevel(priority) Log::Singleton::getInstance()->setLogLevel(priority)
#define ERROR_LOG(msg) Log::Singleton::getInstance()->error(catMsg(msg))
#define WARN_LOG(msg) Log::Singleton::getInstance()->warn(catMsg(msg))
#define INFO_LOG(msg) Log::Singleton::getInstance()->info(catMsg(msg))
#define DEBUG_LOG(msg) Log::Singleton::getInstance()->debug(catMsg(msg))
#define FATAL_LOG(msg) Log::Singleton::getInstance()->fatal(catMsg(msg))

} // WebDesk::Utils

#endif //WEBDESK_UTILS_LOG_H_
