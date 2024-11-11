#include "ConnectionPool.h"
namespace WebDesk {

ConnectionPool::ConnectionPool() {
  parseJsonFile();    //先解析json配置文件

  for (int k = 0; k < minSize_; ++k) {    //创建空闲数据池
    addConnection();
  }

  std::thread producer(&ConnectionPool::produceConnection, this);
  std::thread recycler(&ConnectionPool::recycleConnection, this);

  producer.detach();
  recycler.detach();
}

ConnectionPool::~ConnectionPool() {
  while (!connectionQueue_.empty()) {
    MysqlConn *conn = connectionQueue_.front();
    connectionQueue_.pop();
    delete conn;
  }
}

void
ConnectionPool::parseJsonFile() {

  ip_ = m_config.getValue("db_ip");
  user_ = m_config.getValue("db_user");
  passwd_ = m_config.getValue("db_pwd");
  dbName_ = m_config.getValue("db_name");
  port = std::stoi(m_config.getValue("db_port"));
  minSize_ = std::stoi(m_config.getValue("db_min"));
  maxSize_ = std::stoi(m_config.getValue("db_max"));
  timeout_ = std::stoi(m_config.getValue("db_timeout"));
  maxIdleTime_ = std::stoi(m_config.getValue("db_maxIdleTime"));
}

void
ConnectionPool::addConnection() {
  auto *conn = new MysqlConn;
  if(!conn->connect(ip_, user_, passwd_, dbName_, port)){
    std::cout<<"mysql连接失败"<<std::endl;
  }
  conn->refreshAliveTime();
  connectionQueue_.push(conn);
}

void
ConnectionPool::produceConnection() {
  //调用一次就循环创建连接池，如果达到连接池最高限度就阻塞（wait）
  while (true) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (connectionQueue_.size() >= maxSize_) {
      cond_.wait(lock);
    }
    addConnection();
    cond_.notify_all();
  }
}
void
ConnectionPool::recycleConnection() {
  while (true) {
    //周期性检查
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    std::unique_lock<std::mutex> lock(mutex_);
    while (connectionQueue_.size() > minSize_) {        //保持在最低限度连接池数量，但如果这个连接状态并非长时间没有被使用的话并不会回收
      MysqlConn *conn = connectionQueue_.front();
      if (conn->getAliveTime() >= maxIdleTime_) {  //连接未被使用时间过长，可以销毁
        connectionQueue_.pop();
        delete conn;
      } else {
        break;
      }
    }
  }
}

std::shared_ptr<MysqlConn>
ConnectionPool::getConnection() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (connectionQueue_.empty()) {
    if (cond_.wait_for(lock, std::chrono::milliseconds(timeout_)) == std::cv_status::timeout) {
      if (connectionQueue_.empty()) continue;
    }
  }
  std::shared_ptr<MysqlConn> connPtr(connectionQueue_.front(),
                                     [this](MysqlConn *conn) {
                                       std::lock_guard<std::mutex> locker(mutex_);
                                       conn->refreshAliveTime();
                                       connectionQueue_.push(conn);
                                     });
  connectionQueue_.pop();
  cond_.notify_all();
  return connPtr;
}


}