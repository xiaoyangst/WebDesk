#include "CCommand.h"
#include "CClient.h"
#include "json.hpp"

using json = nlohmann::json;
namespace WebDesk {
CCommand::CCommand() {
  registerMainWindow();
  registerWebDesk();
}

void
CCommand::parseMainWindowsCommands(const std::string &command, const hv::SocketChannelPtr &channel) {
  auto *userInfo = new UserInfo;
  MAINWINDOW task = MAINWINDOW::DEFAULT;

  if (command == "exit") {
    CClient::setRunning(false);
    CClient::setLogin(false);
    return;
  } else if (command == "register") {
    std::cout << "输入用户：";
    getline(std::cin, userInfo->username);
    std::cout << "输入密码：";
    getline(std::cin, userInfo->password);
    task = MAINWINDOW::REGISTER;
  } else if (command == "login") {
    std::cout << "输入用户：";
    getline(std::cin, userInfo->username);
    std::cout << "输入密码：";
    getline(std::cin, userInfo->password);
    task = MAINWINDOW::LOGIN;
  } else if (command == "logout") {
    std::cout << "输入用户：";
    getline(std::cin, userInfo->username);
    task = MAINWINDOW::LOGOUT;
  }

  auto item = MainWindowMap.find(task);
  if (item != MainWindowMap.end()) {
    item->second(*userInfo, channel);
  }
}

void
CCommand::registerMainWindow() {
  MainWindowMap[MAINWINDOW::REGISTER] = [&](UserInfo &userInfo, const hv::SocketChannelPtr &channel) {
    registerUser(userInfo, channel);
  };
  MainWindowMap[MAINWINDOW::LOGIN] = [&](UserInfo &userInfo, const hv::SocketChannelPtr &channel) {
    loginUser(userInfo, channel);
  };
  MainWindowMap[MAINWINDOW::LOGOUT] = [&](UserInfo &userInfo, const hv::SocketChannelPtr &channel) {
    logoutUser(userInfo, channel);
  };
}

void
CCommand::registerUser(UserInfo &info, const hv::SocketChannelPtr &channel) {
  json register_json;
  register_json["login"] = false;
  register_json["status"] = MAINWINDOW::REGISTER;
  register_json["username"] = info.username;
  register_json["password"] = info.password;
  auto data = register_json.dump();

  channel->write(data);
}

void
CCommand::loginUser(UserInfo &info, const hv::SocketChannelPtr &channel) {
  json login_json;
  login_json["login"] = false;
  login_json["status"] = MAINWINDOW::LOGIN;
  login_json["username"] = info.username;
  login_json["password"] = info.password;
  auto data = login_json.dump();

  channel->write(data);
}

void
CCommand::logoutUser(UserInfo &info, const hv::SocketChannelPtr &channel) {
  json logout_json;
  logout_json["login"] = false;
  logout_json["status"] = MAINWINDOW::LOGOUT;
  logout_json["username"] = info.username;
  auto data = logout_json.dump();

  channel->write(data);
}

void
CCommand::parseWebDeskCommands(const std::string &command, const hv::SocketChannelPtr &channel) {

}

void
CCommand::registerWebDesk() {

}
}
