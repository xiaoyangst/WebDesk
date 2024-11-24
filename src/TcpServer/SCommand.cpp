#include <iostream>
#include "SCommand.h"
#include "json.hpp"
#include "UserDao.h"
#include "DirectoryForestDao.h"
#include "public.h"
using json = nlohmann::json;
namespace WebDesk {
SCommand::SCommand() {
  registerMainWindow();
  registerWebDesk();
}

void SCommand::registerMainWindow() {
  MainWindowMap[MAINWINDOW::REGISTER] = [&](std::shared_ptr<UserInfo> &userInfo,
                                            const hv::SocketChannelPtr &channel,
                                            std::shared_ptr<MysqlConn> &mysql_conn) {
    UserDao user_dao(mysql_conn);
    User user;
    auto username = userInfo->username;
    user.username = username;
    user.salt = generate_salt();
    auto crypt_pwd = userInfo->password + user.salt;
    user.password = sha256_string(crypt_pwd);

    bool is_success = user_dao.addUser(user);
    json register_json;
    if (is_success) {
      register_json["status"] = MESSAGE::REGISTERSUCCESS;

      // 注册成功之后，要给该用于添加一个目录 即 /home/用户名
      DirectoryForestDao directory_forest_dao(mysql_conn);
      int userId = user_dao.getUserId(userInfo->username);
      // 准备插入数据
      DirectoryForest directory_forest;
      auto file_path = "/home/" + username;
      directory_forest.file_path = file_path;
      directory_forest.file_name = username;
      directory_forest.belong_user = userId;
      directory_forest.father_id = -1;  // -1 表示 根目录 home 的 ID，实际上数据库中不会有 home 目录的记录
      directory_forest_dao.addDir(directory_forest);
    } else {
      register_json["status"] = MESSAGE::REGISTERFAIL;
      std::cout << "失败" << std::endl;
    }
    auto data = register_json.dump();
    channel->write(data);
  };

  MainWindowMap[MAINWINDOW::LOGIN] = [&](std::shared_ptr<UserInfo> &userInfo,
                                         const hv::SocketChannelPtr &channel, std::shared_ptr<MysqlConn> &mysql_conn) {
    UserDao user_dao(mysql_conn);
    json login_json;
    int userId;
    if ((userId = user_dao.getUserId(userInfo->username)) == -1) { // 用户不存在
      login_json["status"] = MESSAGE::USERNOEXIST;
      auto data = login_json.dump();
      channel->write(data);
      return;
    }

    auto crypt_pwd = userInfo->password + user_dao.getSalt(userId);
    auto password = sha256_string(crypt_pwd);
    if (password != user_dao.getPwd(userId)) {  // 密码错误
      login_json["status"] = MESSAGE::PWDERROR;
      auto data = login_json.dump();
      channel->write(data);
      return;
    }

    std::string token;
    JwtEncode(token, userInfo->username);
    login_json["username"] = userInfo->username;
    login_json["password"] = userInfo->password;
    login_json["token"] = token;
    login_json["status"] = MESSAGE::LOGINSUCCESS;
    auto data = login_json.dump();
    channel->write(data);
  };

  MainWindowMap[MAINWINDOW::LOGOUT] = [&](std::shared_ptr<UserInfo> &userInfo,
                                          const hv::SocketChannelPtr &channel, std::shared_ptr<MysqlConn> &mysql_conn) {
    UserDao user_dao(mysql_conn);
    json logout_json;
    int userId;
    if ((userId = user_dao.getUserId(userInfo->username)) == -1) { // 用户不存在
      logout_json["status"] = MESSAGE::USERNOEXIST;
      auto data = logout_json.dump();
      channel->write(data);
      return;
    }
    bool death = true;
    bool is_success = user_dao.updateUser(userId, death);
    if (is_success) {
      logout_json["status"] = MESSAGE::LOGOUTSUCCESS;
    } else {
      logout_json["status"] = MESSAGE::LOGOUTFAIL;
    }
    auto data = logout_json.dump();
    channel->write(data);
  };
}
void SCommand::registerWebDesk() {
  WebDeskMap[WEBDESK::MKDIR] = [&](const json &data_json,
                                   const hv::SocketChannelPtr &channel,
                                   std::shared_ptr<MysqlConn> &mysql_conn) {
    DirectoryForestDao directory_forest_dao(mysql_conn);
    UserDao user_dao(mysql_conn);
    json df_json;
    // 获取用户 id
    auto user = data_json["user"].get<std::string>();
    int userId = user_dao.getUserId(user);
    // 准备插入数据
    DirectoryForest directory_forest;
    auto file_path = data_json["filepath"].get<std::string>();
    auto file_name = data_json["filename"].get<std::string>();
    int fatherID = directory_forest_dao.getId(file_path);
    directory_forest.file_path = file_path + "/" + file_name;
    directory_forest.file_name = file_name;
    directory_forest.belong_user = userId;
    directory_forest.father_id = fatherID;
    if (directory_forest_dao.addDir(directory_forest)) {
      df_json["status"] = MESSAGE::ADDDIRSUCCESS;
    } else {
      df_json["status"] = MESSAGE::ADDDIRFAIL;
    }
    auto data = df_json.dump();
    channel->write(data);
  };

  WebDeskMap[WEBDESK::REMOVE] = [&](const json &data_json,
                                    const hv::SocketChannelPtr &channel,
                                    std::shared_ptr<MysqlConn> &mysql_conn) {
    DirectoryForestDao directory_forest_dao(mysql_conn);
    json df_json;
    auto file_path = data_json["filepath"].get<std::string>();
    auto file_name = data_json["filename"].get<std::string>();
    auto file_type = data_json["filetype"].get<bool>();
    auto path = file_path + "/" + file_name;
    if(file_type) {
      // 找到文件+标记为未删除 --》 删除
      int file_id = directory_forest_dao.getDelId(path);
      if (file_id == -1) {
        df_json["status"] = MESSAGE::REMOVENOEXIST;
      } else {
        if (directory_forest_dao.delFile(file_id)) {
          df_json["status"] = MESSAGE::REMOVESUCCESS;
        } else {
          df_json["status"] = MESSAGE::REMOVEFAIL;
        }
      }
    }else {
      int file_id = directory_forest_dao.getDelId(path);
      if (file_id == -1) {
        df_json["status"] = MESSAGE::REMOVENOEXIST;
      } else {
        if (directory_forest_dao.delDir(path)) {
          df_json["status"] = MESSAGE::REMOVESUCCESS;
        } else {
          df_json["status"] = MESSAGE::REMOVEFAIL;
        }
      }
      auto data = df_json.dump();
      channel->write(data);
    }
  };

}
}