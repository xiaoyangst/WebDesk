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
    UserDao user_dao(mysql_conn);
    User user;
    user.username = userInfo->username;
    user.salt = generate_salt();
    auto crypt_pwd = userInfo->password + user.salt;
    user.password = sha256_string(crypt_pwd);

    bool is_success = user_dao.addUser(user);
    json register_json;
    if (is_success) {
      register_json["status"] = MESSAGE::REGISTERSUCCESS;
    } else {
      register_json["status"] = MESSAGE::REGISTERFAIL;
      std::cout<<"失败"<<std::endl;
    }
    auto data = register_json.dump();
    channel->write(data);
  };

  MainWindowMap[MAINWINDOW::LOGIN] = [&](std::shared_ptr<UserInfo> &userInfo,
      const hv::SocketChannelPtr &channel,std::shared_ptr<MysqlConn> &mysql_conn) {
    UserDao user_dao(mysql_conn);
    json login_json;
    int userId;
    if ((userId = user_dao.getUserId(userInfo->username)) == -1){ // 用户不存在
      login_json["status"] = MESSAGE::USERNOEXIST;
      auto data = login_json.dump();
      channel->write(data);
      return ;
    }

    auto crypt_pwd = userInfo->password + user_dao.getSalt(userId);
    auto password = sha256_string(crypt_pwd);
    if (password != user_dao.getPwd(userId)){  // 密码错误
      login_json["status"] = MESSAGE::PWDERROR;
      auto data = login_json.dump();
      channel->write(data);
      return ;
    }

//    std::string token;
//    JwtEncode(token,userInfo->username);
//    std::cout<<"token = "<<token<<std::endl;
//    if (JwtDecode(token)){
//      std::cout<<"合法用户"<<std::endl;
//    }
//    login_json["token"] = token;
    login_json["status"] = MESSAGE::LOGINSUCCESS;
    auto data = login_json.dump();
    channel->write(data);
  };

  MainWindowMap[MAINWINDOW::LOGOUT] = [&](std::shared_ptr<UserInfo> &userInfo,
      const hv::SocketChannelPtr &channel,std::shared_ptr<MysqlConn> &mysql_conn) {
    UserDao user_dao(mysql_conn);
    json logout_json;
    int userId;
    if ((userId = user_dao.getUserId(userInfo->username)) == -1){ // 用户不存在
      logout_json["status"] = MESSAGE::USERNOEXIST;
      auto data = logout_json.dump();
      channel->write(data);
      return ;
    }
    bool death = true;
    bool is_success = user_dao.updateUser(userId,death);
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
