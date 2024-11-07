#include "Log.h"

namespace WebDesk {

Log::Log()
    : m_rootCategory(log4cpp::Category::getRoot().getInstance("rootCategory")) {
  // rootCategory 下 添加子 Category

  // 输出到控制台
  auto *osAppender = new log4cpp::OstreamAppender("console", &std::cout);
  auto *pLayoutA = new log4cpp::PatternLayout();
  pLayoutA->setConversionPattern("%d: %p %c %x: %m%n");
  osAppender->setLayout(pLayoutA);
  m_rootCategory.addAppender(osAppender);
  m_rootCategory.setPriority(log4cpp::Priority::DEBUG);

  // 写入到日志文件
  auto *fileAppender = new log4cpp::FileAppender("fileAppender", "log");
  auto *pLayoutB = new log4cpp::PatternLayout();
  pLayoutB->setConversionPattern("%d: %p %c %x: %m%n");
  fileAppender->setLayout(pLayoutB);
  m_rootCategory.addAppender(fileAppender);
  m_rootCategory.setPriority(log4cpp::Priority::DEBUG);

  // 日志回滚
  auto *rollingFileAppender = new log4cpp::RollingFileAppender(
      "rollingFileAppender", "logrolling.log", 5 * 1024, 5); // 5M（超过就新创建一个）,备份文件5个
  auto *pLayoutC = new log4cpp::PatternLayout();
  pLayoutC->setConversionPattern("%d: %p %c %x: %m%n");
  rollingFileAppender->setLayout(pLayoutC);
  m_rootCategory.addAppender(rollingFileAppender);
  m_rootCategory.setPriority(log4cpp::Priority::DEBUG);
}
Log::~Log() {
  log4cpp::Category::shutdown();
}

void
Log::setLogLevel(log4cpp::Priority::Value priority) {
  switch (priority) {
    case log4cpp::Priority::FATAL:
      m_rootCategory.setPriority(log4cpp::Priority::FATAL);
      break;
    case log4cpp::Priority::ERROR:
      m_rootCategory.setPriority(log4cpp::Priority::ERROR);
      break;
    case log4cpp::Priority::WARN:
      m_rootCategory.setPriority(log4cpp::Priority::WARN);
      break;
    case log4cpp::Priority::INFO:
      m_rootCategory.setPriority(log4cpp::Priority::INFO);
      break;
    case log4cpp::Priority::DEBUG:
      m_rootCategory.setPriority(log4cpp::Priority::DEBUG);
      break;
    default:
      m_rootCategory.setPriority(log4cpp::Priority::DEBUG);
      break;
  }
}

std::string
Log::int2string(int lineNumber) {
  std::ostringstream oss;
  oss << lineNumber;
  return oss.str();
}

void
Log::fatal(const char *msg) {
  m_rootCategory.fatal(msg);
}
void
Log::error(const char *msg) {
  m_rootCategory.error(msg);
}
void
Log::warn(const char *msg) {
  m_rootCategory.warn(msg);
}
void
Log::info(const char *msg) {
  m_rootCategory.info(msg);
}
void
Log::debug(const char *msg) {
  m_rootCategory.debug(msg);
}
} // WebDesk::Utils