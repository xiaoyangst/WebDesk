#include <string>
#include <utility>
#include "SServer.h"
#include "Log.h"
#include "json.hpp"
#include "UserDao.h"

using json = nlohmann::json;
namespace WebDesk {
SServer::SServer(int port, std::string ip)
    : m_port(port), m_ip(std::move(ip))
    , m_mysqlPool(ConnectionPool::getInstance())
    {
  m_listenfd = srv.createsocket(port, m_ip.c_str());
  if (m_listenfd < 0) {
    ERROR_LOG("create socket failed");
    return;
  }
  printf("server listen on port %d, listenfd=%d ...\n", port, m_listenfd);

  waitConnect();
}

void
SServer::waitConnect() {
  srv.onConnection = [](const hv::SocketChannelPtr &channel) {
    std::string peerAddr = channel->peeraddr();
    if (channel->isConnected()) {
      printf("%s connected! connfd=%d\n", peerAddr.c_str(), channel->fd());
    } else {
      printf("%s disconnected! connfd=%d\n", peerAddr.c_str(), channel->fd());
    }
  };
  srv.onMessage = [this](const hv::SocketChannelPtr &channel, hv::Buffer *buf) {
    auto data = std::string((char *) buf->data());
    json data_json = json::parse(data);
    bool is_login = data_json["login"].get<bool>();
    if (!is_login) {
      auto type = data_json["status"].get<MAINWINDOW>();
      std::shared_ptr<UserInfo> userInfo = std::make_shared<UserInfo>();
      userInfo->username = data_json["username"].get<std::string>();
      userInfo->password = data_json["password"].get<std::string>();
      auto item = m_command.MainWindowMap.find(type);
      if (item != m_command.MainWindowMap.end()) {
        auto sql = m_mysqlPool->getConnection();
        item->second(userInfo, channel,sql);
      }
    } else {
      auto type = data_json["status"].get<WEBDESK>();
    }
  };
  srv.setThreadNum(4);
  srv.start();

  while (getchar() != '\n');
}
}