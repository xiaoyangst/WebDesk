/**
  ******************************************************************************
  * @file           : CCommand.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/4
  ******************************************************************************
  */

#ifndef WEBDESK_SRC_TCPCLIENT_CCOMMAND_H_
#define WEBDESK_SRC_TCPCLIENT_CCOMMAND_H_

#include <string>
#include <unordered_map>
#include <iostream>
#include <functional>
#include "head.h"
#include "Channel.h"
namespace WebDesk {
class CCommand {
 public:
  explicit CCommand();
  ~CCommand() = default;
  // 解析命令
  void parseMainWindowsCommands(const std::string &command, const hv::SocketChannelPtr &channel);
  void parseWebDeskCommands(const std::string &command, const hv::SocketChannelPtr &channel);
 private:
  // 注册回调
  void registerMainWindow();
  void registerWebDesk();
 private:
  void registerUser(UserInfo &info, const hv::SocketChannelPtr &channel);
  void loginUser(UserInfo &info, const hv::SocketChannelPtr &channel);
  void logoutUser(UserInfo &info, const hv::SocketChannelPtr &channel);

 private:
  std::unordered_map<MAINWINDOW, std::function<void(UserInfo &userInfo, const hv::SocketChannelPtr &channel)>>
      MainWindowMap;
  std::unordered_map<WEBDESK, std::function<void(void)>> WebDeskMap;
 public:
  CCommand(const CCommand &) = delete;
  CCommand &operator=(const CCommand &) = delete;
};
}
#endif //WEBDESK_SRC_TCPCLIENT_CCOMMAND_H_
