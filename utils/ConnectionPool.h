/**
  ******************************************************************************
  * @file           : ConnectionPool.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/7
  ******************************************************************************
  */

#ifndef WEBDESK_UTILS_CONNECTIONPOOL_H_
#define WEBDESK_UTILS_CONNECTIONPOOL_H_

#include <queue>    //存放连接的容器
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "MysqlConn.h"
#include "Singleton.h"
#include "Config.h"

namespace WebDesk {
class ConnectionPool : public Singleton<ConnectionPool>, public std::enable_shared_from_this<ConnectionPool>{
 friend class Singleton<ConnectionPool>;
 public:
  ConnectionPool();
  ~ConnectionPool();
  std::shared_ptr<MysqlConn> getConnection(); // 获取一个可以和数据库通信的 conn
  ConnectionPool(const ConnectionPool&) = delete;
  ConnectionPool& operator=(const ConnectionPool&) = delete;
 private:
  void parseJsonFile();        //MySQL配置信息解析
  void produceConnection();
  void recycleConnection();
  void addConnection();

  std::string ip_;
  std::string user_;
  std::string passwd_;
  std::string dbName_;
  int port;

  int minSize_;
  int maxSize_;
  int timeout_;
  int maxIdleTime_;

  std::queue<MysqlConn*> connectionQueue_;
  std::mutex mutex_;
  std::condition_variable cond_;

  Config m_config;
};
}
#endif //WEBDESK_UTILS_CONNECTIONPOOL_H_
