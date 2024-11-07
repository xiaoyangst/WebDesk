#include <iostream>
#include "SCommand.h"
#include "json.hpp"
#include "UserDao.h"
#include "public.h"
using json = nlohmann::json;
namespace WebDesk {
SCommand::SCommand() {
  registerMainWindow();
}

void SCommand::registerMainWindow() {
  MainWindowMap[MAINWINDOW::REGISTER] = [&](std::shared_ptr<UserInfo> &userInfo,
      const hv::SocketChannelPtr &channel,std::shared_ptr<MysqlConn> &mysql_conn) {
    std::shared_ptr<UserDao> user_dao = std::make_shared<UserDao>(mysql_conn);
    std::shared_ptr<User> user = std::make_shared<User>();
    user->username = userInfo->username;
    user->death = false;
    user->salt = generate_salt();
    auto crypt_pwd = userInfo->password + user->salt;
    user->password = sha256_string(crypt_pwd);
    bool is_success = user_dao->addUser(user);
    json register_json;
    if (is_success) {
      register_json["status"] = MESSAGE::REGISTERSUCCESS;
    } else {
      register_json["status"] = MESSAGE::REGISTERFAIL;
    }
    auto data = register_json.dump();
    channel->write(data);
  };

  MainWindowMap[MAINWINDOW::LOGIN] = [&](std::shared_ptr<UserInfo> &userInfo,
      const hv::SocketChannelPtr &channel,std::shared_ptr<MysqlConn> &mysql_conn) {
    std::shared_ptr<UserDao> user_dao = std::make_shared<UserDao>(mysql_conn);
    int userId = 1;
    bool death = true;
    bool is_success = user_dao->updateUser(userId,death);
    json login_json;
    if (is_success) {
      login_json["status"] = MESSAGE::LOGINSUCCESS;
    } else {
      login_json["status"] = MESSAGE::LOGINFAIL;
    }
    auto data = login_json.dump();
    channel->write(data);
  };

  MainWindowMap[MAINWINDOW::LOGOUT] = [&](std::shared_ptr<UserInfo> &userInfo,
      const hv::SocketChannelPtr &channel,std::shared_ptr<MysqlConn> &mysql_conn) {
    std::shared_ptr<UserDao> user_dao = std::make_shared<UserDao>(mysql_conn);
    int userId = 1;
    bool is_success = user_dao->deleteUser(userId);
    json logout_json;
    if (is_success) {
      logout_json["status"] = MESSAGE::LOGOUTSUCCESS;
    } else {
      logout_json["status"] = MESSAGE::LOGOUTFAIL;
    }
    auto data = logout_json.dump();
    channel->write(data);
  };
}
}
