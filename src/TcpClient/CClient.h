/**
  ******************************************************************************
  * @file           : CClient.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/4
  ******************************************************************************
  */

#ifndef WEBDESK_SRC_TCPCLIENT_CCLIENT_H_
#define WEBDESK_SRC_TCPCLIENT_CCLIENT_H_

#include <iostream>
#include "CCommand.h"
#include "hv/TcpClient.h"
#include "json.hpp"
#include "public.h"

using json = nlohmann::json;
namespace WebDesk {
class CClient {
  friend class CCommand;
 public:
  CClient(int port, std::string m_ip);

  void connectServer();
  void showMainWindow();
  void readCommands(const hv::SocketChannelPtr &channel);

  void taskMainWindow(MESSAGE type, const hv::SocketChannelPtr &channel,const json& data_json);

  void taskWebDesk(MESSAGE type, const hv::SocketChannelPtr &channel,const json& data_json);

  static void setLogin(bool login);
  static void setRunning(bool running);
  static std::string getCurPath();
  static std::string getUser();
  int getfd() const;
 private:
  void downloadTask();
  void uploadTask();
 private:
  int m_port;
  std::string m_ip;
  static bool is_login;
  static bool is_running;
  CCommand m_command;
  hv::TcpClient tcp_client;
  int m_connfd;
  static std::shared_ptr<UserInfo> m_user;
 public:
  CClient(const CClient &) = delete;
  CClient &operator=(const CClient &) = delete;
};
}


#endif //WEBDESK_SRC_TCPCLIENT_CCLIENT_H_
