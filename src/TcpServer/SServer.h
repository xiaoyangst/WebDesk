/**
  ******************************************************************************
  * @file           : SServer.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/6
  ******************************************************************************
  */

#ifndef WEBDESK_SRC_TCPSERVER_SSERVER_H_
#define WEBDESK_SRC_TCPSERVER_SSERVER_H_

#include "hv/TcpServer.h"
#include "ConnectionPool.h"
#include "head.h"
#include "SCommand.h"
namespace WebDesk {
class SServer {
 public:
  SServer(int port, const char *ip);
 private:
  void waitConnect();
 private:
  int m_port;
  const char *m_ip;
  int m_listenfd;
  SCommand m_command;
  std::shared_ptr<ConnectionPool> m_mysqlPool;
  hv::TcpServer srv;
};
}
#endif //WEBDESK_SRC_TCPSERVER_SSERVER_H_
