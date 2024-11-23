/**
  ******************************************************************************
  * @file           : SCommand.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/6
  ******************************************************************************
  */

#ifndef WEBDESK_SRC_TCPSERVER_SCOMMAND_H_
#define WEBDESK_SRC_TCPSERVER_SCOMMAND_H_

#include "head.h"
#include <functional>
#include "hv/TcpServer.h"
#include "MysqlConn.h"
#include "User.h"
#include "json.hpp"
using json = nlohmann::json;
namespace WebDesk {
class SCommand {
  friend class SServer;
 public:
  SCommand();
 private:
  // 注册回调
  void registerMainWindow();
  void registerWebDesk();

 private:
  std::unordered_map<MAINWINDOW, std::function<void(std::shared_ptr<UserInfo> &userInfo,
                                                    const hv::SocketChannelPtr &channel,
                                                    std::shared_ptr<MysqlConn> &mysql_conn)>>
      MainWindowMap;
  std::unordered_map<WEBDESK, std::function<void(const json& data_json,
                                                 const hv::SocketChannelPtr &channel,
                                                 std::shared_ptr<MysqlConn> &mysql_conn)>>
      WebDeskMap;
};
}
#endif //WEBDESK_SRC_TCPSERVER_SCOMMAND_H_
