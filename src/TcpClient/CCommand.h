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
  void registerUser(const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel);
  void loginUser(const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel);
  void logoutUser(const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel);

  void ls(const std::string& file, const hv::SocketChannelPtr &channel);
  void mkdirDir(const std::string& file, const hv::SocketChannelPtr &channel);
  void removeFileDir(const std::string& file, const hv::SocketChannelPtr &channel);
  void uploadFile(const std::string& file, const hv::SocketChannelPtr &channel);
  void downloadFile(const std::string& file, const hv::SocketChannelPtr &channel);
  void cdDir(const std::string& file, const hv::SocketChannelPtr &channel);
 private:
  std::unordered_map<MAINWINDOW, std::function<void(std::string user,std::string pwd, const hv::SocketChannelPtr &channel)>>
      MainWindowMap;
  std::unordered_map<WEBDESK, std::function<void(std::string file, const hv::SocketChannelPtr &channel)>>
      WebDeskMap;
 public:
  CCommand(const CCommand &) = delete;
  CCommand &operator=(const CCommand &) = delete;
};
}
#endif //WEBDESK_SRC_TCPCLIENT_CCOMMAND_H_
