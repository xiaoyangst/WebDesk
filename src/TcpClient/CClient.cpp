#include "CClient.h"

#include <utility>

namespace WebDesk {
bool CClient::is_running = true;
bool CClient::is_login = false;
std::shared_ptr<UserInfo> CClient::m_user = nullptr;

CClient::CClient(int port, std::string ip)
    : m_port(port), m_ip(std::move(ip)) {
  m_user = std::make_shared<UserInfo>();
  m_connfd = tcp_client.createsocket(m_port, m_ip.c_str());
  if (m_connfd < 0) {
    printf("connfd failed\n");
    return;
  }
  connectServer();
}

void
CClient::showMainWindow() {
  std::cout << "\n************************网盘客户端系统************************\n" << std::endl;
  if (is_login) {
    std::cout << "pwd       查看当前目录" << std::endl;
    std::cout << "ls        查看当前目录下所有文件或文件夹" << std::endl;
    std::cout << "mkdir     创建目录" << std::endl;
    std::cout << "remove    删除文件夹或文件" << std::endl;
    std::cout << "upload    上传文件" << std::endl;
    std::cout << "download  下载文件" << std::endl;
    std::cout << "cd        进入目录" << std::endl;
    std::cout << "quit      退出" << std::endl;
  } else {
    std::cout << "exit-退出  register-注册  login-登陆  logout-注销" << std::endl;
  }
  std::cout << "\n************************网盘客户端系统************************\n" << std::endl;
}

void
CClient::readCommands(const hv::SocketChannelPtr &channel) {
  showMainWindow();
  std::string input;
  if (!is_login) {
    std::cout << "请输入命令>";
    getline(std::cin, input);
    if (input == "exit" || input == "register" || input == "login" || input == "logout") {
      m_command.parseMainWindowsCommands(input, channel);
    } else {
      readCommands(channel);
    }
  } else {
    std::cout <<"[" <<m_user->current_path << "]--> ";
    getline(std::cin, input);
    if(input == "pwd"){
      std::cout<<">"<<m_user->current_path<<std::endl;
      readCommands(channel);
    } else if (input == "quit"){
      is_login = false;
      readCommands(channel);
    }else if (input == "ls" || input == "mkdir" || input == "remove" ||
        input == "upload" || input == "download" || input == "cd") {
      m_command.parseWebDeskCommands(input, channel);
    }else{
      readCommands(channel);
    }
  }
}

void
CClient::connectServer() {
  std::cout << "client connect server ip = " << m_ip << " port = " << m_port << std::endl;
  // 连接建立回调
  tcp_client.onConnection = [&](const hv::SocketChannelPtr &channel) {
    std::string peeraddr = channel->peeraddr();
    if (channel->isConnected()) {
      printf("onConnection connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
      readCommands(channel);
    } else {
      printf("onConnection disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
      printf("连接断开\n");
      return;
    }
  };

  // 业务回调
  tcp_client.onMessage = [&](const hv::SocketChannelPtr &channel, hv::Buffer *buf) {
    std::string peeraddr = channel->peeraddr();
    printf("onMessage connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
    auto data = std::string((char *) buf->data());
    json data_json = json::parse(data);
    auto type = data_json["status"].get<MESSAGE>();
    if (!is_login) { // 登录业务
      taskMainWindow(type, channel,data_json);
    } else {
      taskWebDesk(type, channel,data_json);
    }
  };

  tcp_client.start();
  while (is_running);
}

void
CClient::setLogin(bool login) {
  is_login = login;
}

void
CClient::setRunning(bool running) {
  is_running = running;
}

int
CClient::getfd() const {
  return m_connfd;
}

void
CClient::taskMainWindow(MESSAGE type, const hv::SocketChannelPtr &channel,const json& data_json) {
  switch (type) {
    case MESSAGE::LOGINSUCCESS: {
      printColor("登陆成功！！！", GREEN);
      is_login = true;
      m_user->token = data_json["token"].get<std::string>();
      m_user->username = data_json["username"].get<std::string>();
      m_user->password = data_json["password"].get<std::string>();
      m_user->current_path = "/home/" + data_json["username"].get<std::string>();
      readCommands(channel);
      break;
    }
    case MESSAGE::USERNOEXIST: {
      printColor("用户不存在！！！", RED);
      readCommands(channel);
      break;
    }
    case MESSAGE::PWDERROR: {
      printColor("密码错误！！！", RED);
      readCommands(channel);
      break;
    }
    case MESSAGE::LOGINFAIL: {
      printColor("登陆失败！！！", RED);
      readCommands(channel);
      break;
    }
    case MESSAGE::REGISTERSUCCESS: {
      printColor("注册成功！！！", GREEN);
      readCommands(channel);
      break;
    }
    case MESSAGE::REGISTERFAIL: {
      printColor("注册失败！！！", RED);
      readCommands(channel);
      break;
    }
    case MESSAGE::LOGOUTSUCCESS: {
      printColor("注销成功！！！", GREEN);
      readCommands(channel);
      break;
    }
    case MESSAGE::LOGOUTFAIL: {
      printColor("注销失败！！！", RED);
      readCommands(channel);
      break;
    }
    default: {
      readCommands(channel);
      break;
    }
  }
}

void
CClient::taskWebDesk(MESSAGE type, const hv::SocketChannelPtr &channel,const json& data_json) {
  switch (type) {
    case MESSAGE::ADDDIRSUCCESS:{
      printColor("创建文件夹成功！！！", GREEN);
      readCommands(channel);
      break;
    }
    case MESSAGE::ADDDIRFAIL:{
      printColor("创建文件夹失败！！！",RED);
      readCommands(channel);
      break;
    }
    case MESSAGE::REMOVESUCCESS:{
      printColor("删除文件/文件夹成功！！！",GREEN);
      readCommands(channel);
      break;
    }
    case MESSAGE::REMOVEFAIL:{
      printColor("删除文件/文件夹失败！！！",RED);
      readCommands(channel);
      break;
    }
    case MESSAGE::REMOVENOEXIST:{
      printColor("不要尝试删除不存在的文件/文件夹！！！",RED);
      readCommands(channel);
      break;
    }
    default:
      std::cout << "没有返回" << std::endl;
  }
}

std::string
CClient::getCurPath() {
  return m_user->current_path;
}
std::string CClient::getUser() {
  return m_user->username;
}

}