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
  std::string username;
  std::string password;
  MAINWINDOW task = MAINWINDOW::DEFAULT;
  if (command == "exit") {
    CClient::setRunning(false);
    CClient::setLogin(false);
    return;
  } else if (command == "register") {
    std::cout << "输入用户：";
    getline(std::cin, username);
    std::cout << "输入密码：";
    getline(std::cin, password);
    task = MAINWINDOW::REGISTER;
  } else if (command == "login") {
    std::cout << "输入用户：";
    getline(std::cin, username);
    std::cout << "输入密码：";
    getline(std::cin, password);
    task = MAINWINDOW::LOGIN;
  } else if (command == "logout") {
    std::cout << "输入用户：";
    getline(std::cin, username);
    task = MAINWINDOW::LOGOUT;
  }

  auto item = MainWindowMap.find(task);
  if (item != MainWindowMap.end()) {
    item->second(username,password,channel);
  }
}

void
CCommand::registerMainWindow() {
  MainWindowMap[MAINWINDOW::REGISTER] = [&](const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel) {
    registerUser(user,pwd, channel);
  };
  MainWindowMap[MAINWINDOW::LOGIN] = [&](const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel) {
    loginUser(user,pwd, channel);
  };
  MainWindowMap[MAINWINDOW::LOGOUT] = [&](const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel) {
    logoutUser(user,pwd, channel);
  };
}

void
CCommand::parseWebDeskCommands(const std::string &command, const hv::SocketChannelPtr &channel) {
  std::string file;
  WEBDESK task = WEBDESK::DEFAULT;
  if(command == "ls"){
    task = WEBDESK::LS;
  } else if (command == "mkdir"){
    std::cout << "输入文件夹名称：";
    getline(std::cin, file);
    task = WEBDESK::MKDIR;
  }else if (command == "remove"){
    std::cout << "输入文件夹/文件名称：";
    getline(std::cin, file);
    task = WEBDESK::REMOVE;
  }else if (command == "upload"){
    std::cout << "输入文件名称：";
    getline(std::cin, file);
    task = WEBDESK::UPLOAD;
  }else if (command == "download"){
    std::cout << "输入文件名称：";
    getline(std::cin, file);
    task = WEBDESK::DOWNLOAD;
  }else if (command == "cd"){
    std::cout << "输入文件夹名称：";
    getline(std::cin, file);
    task = WEBDESK::DOWNLOAD;
  }

  auto item = WebDeskMap.find(task);
  if (item != WebDeskMap.end()) {
    item->second(file,channel);
  }
}

void
CCommand::registerWebDesk() {
  WebDeskMap[WEBDESK::LS] = [&](const std::string& file, const hv::SocketChannelPtr &channel){
    ls(file,channel);
  };
  WebDeskMap[WEBDESK::MKDIR] = [&](const std::string& file, const hv::SocketChannelPtr &channel){
    mkdirDir(file,channel);
  };
  WebDeskMap[WEBDESK::REMOVE] = [&](const std::string& file, const hv::SocketChannelPtr &channel){
    removeFileDir(file,channel);
  };
  WebDeskMap[WEBDESK::UPLOAD] = [&](const std::string& file, const hv::SocketChannelPtr &channel){
    uploadFile(file,channel);
  };
  WebDeskMap[WEBDESK::DOWNLOAD] = [&](const std::string& file, const hv::SocketChannelPtr &channel){
    downloadFile(file,channel);
  };
  WebDeskMap[WEBDESK::CD] = [&](const std::string& file, const hv::SocketChannelPtr &channel){
    cdDir(file,channel);
  };
}

void
CCommand::registerUser(const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel) {
  json register_json;
  register_json["login"] = false;
  register_json["status"] = MAINWINDOW::REGISTER;
  register_json["username"] = user;
  register_json["password"] = pwd;
  auto data = register_json.dump();

  channel->write(data);
}

void
CCommand::loginUser(const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel) {
  json login_json;
  login_json["login"] = false;
  login_json["status"] = MAINWINDOW::LOGIN;
  login_json["username"] = user;
  login_json["password"] = pwd;
  auto data = login_json.dump();

  channel->write(data);
}

void
CCommand::logoutUser(const std::string& user,const std::string& pwd, const hv::SocketChannelPtr &channel) {
  json logout_json;
  logout_json["login"] = false;
  logout_json["status"] = MAINWINDOW::LOGOUT;
  logout_json["username"] = user;
  logout_json["password"] = pwd;
  auto data = logout_json.dump();

  channel->write(data);
}

void CCommand::ls(const std::string &file, const hv::SocketChannelPtr &channel) {
  json ls_json;
  ls_json["login"] = true;
  ls_json["command"] = WEBDESK::LS;
  ls_json["user"] = CClient::getUser();
  ls_json["file"] = file;
  auto data = ls_json.dump();

  channel->write(data);
}
void CCommand::mkdirDir(const std::string &file, const hv::SocketChannelPtr &channel) {
  json mkdir_json;
  mkdir_json["login"] = true;
  mkdir_json["command"] = WEBDESK::MKDIR;
  mkdir_json["filename"] = file;
  mkdir_json["user"] = CClient::getUser();
  mkdir_json["filepath"] = CClient::getCurPath();
  auto data = mkdir_json.dump();

  channel->write(data);
}
void CCommand::removeFileDir(const std::string &file, const hv::SocketChannelPtr &channel) {
  json remove_json;
  remove_json["login"] = true;
  remove_json["command"] = WEBDESK::REMOVE;
  remove_json["user"] = CClient::getUser();
  remove_json["file"] = file;
  auto data = remove_json.dump();

  channel->write(data);
}
void CCommand::uploadFile(const std::string &file, const hv::SocketChannelPtr &channel) {
  json upload_json;
  upload_json["login"] = true;
  upload_json["command"] = WEBDESK::UPLOAD;
  upload_json["user"] = CClient::getUser();
  upload_json["file"] = file;
  auto data = upload_json.dump();

  channel->write(data);
}
void CCommand::downloadFile(const std::string &file, const hv::SocketChannelPtr &channel) {
  json download_json;
  download_json["login"] = true;
  download_json["command"] = WEBDESK::DOWNLOAD;
  download_json["user"] = CClient::getUser();
  download_json["file"] = file;
  auto data = download_json.dump();

  channel->write(data);
}
void CCommand::cdDir(const std::string &file, const hv::SocketChannelPtr &channel) {
  json cd_json;
  cd_json["login"] = true;
  cd_json["command"] = WEBDESK::CD;
  cd_json["user"] = CClient::getUser();
  cd_json["file"] = file;
  auto data = cd_json.dump();

  channel->write(data);
}

}
