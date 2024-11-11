#include "CClient.h"

#include <utility>
#include "json.hpp"
#include "public.h"

using json = nlohmann::json;
namespace WebDesk {
bool CClient::is_running = true;
bool CClient::is_login = false;

CClient::CClient(int port, std::string ip)
    : m_port(port), m_ip(std::move(ip)) {
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
    std::cout << "quit      退出" << std::endl;
  } else {
    std::cout << "exit-退出  register-注册  login-登陆  logout-注销" << std::endl;
  }
  std::cout << "\n************************网盘客户端系统************************\n" << std::endl;
}

void
CClient::readCommands(const hv::SocketChannelPtr &channel) {
  showMainWindow();
  // 读取用户命令
  std::string input;
  std::cout << "请输入命令>";
  getline(std::cin, input);
  // 解析命令类型,根据命令执行对应任务
  if (!is_login) {
    if (input == "exit" || input == "register" || input == "login" || input == "logout") {
      m_command.parseMainWindowsCommands(input, channel);
    } else {
      readCommands(channel);
    }
  } else {
    m_command.parseWebDeskCommands(input, channel);
  }
}

void
CClient::connectServer() {
  std::cout << "client connect server ip = " << m_ip << " port = " << m_port << std::endl;
  tcp_client.onConnection = [&](const hv::SocketChannelPtr &channel) {
    if (channel->isConnected()) {
      readCommands(channel);
    } else {
      printf("连接断开\n");
      return;
    }
  };

  tcp_client.onMessage = [&](const hv::SocketChannelPtr &channel, hv::Buffer *buf) {
    // 业务处理
    auto data = std::string((char *) buf->data());
    json data_json = json::parse(data);
    auto type = data_json["status"].get<MESSAGE>();
    if (!is_login) { // 登录业务
      taskMainWindow(type, channel);
    } else {
      taskWebDesk(type, channel);
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
CClient::taskMainWindow(MESSAGE type, const hv::SocketChannelPtr &channel) {
  switch (type) {
    case MESSAGE::LOGINSUCCESS: {
      printColor("登陆成功！！！", GREEN);
      is_login = true;
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
void CClient::taskWebDesk(MESSAGE type, const hv::SocketChannelPtr &channel) {

}
}