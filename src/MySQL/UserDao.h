/**
  ******************************************************************************
  * @file           : UserDao.h
  * @author         : xy
  * @brief          : user表的接口
  * @attention      : None
  * @date           : 2024/11/6
  ******************************************************************************
  */

#ifndef WEBDESK_SRC_MYSQL_USERDAO_H_
#define WEBDESK_SRC_MYSQL_USERDAO_H_

#include "User.h"
#include <mysql/mysql.h>
#include <vector>
#include <optional>
#include <memory>
#include "Config.h"
#include "MysqlConn.h"
using namespace WebDesk;
class UserDao {
 public:
  explicit UserDao(std::shared_ptr<MysqlConn> &conn);
  ~UserDao() = default;
  bool addUser(const User& user); // 注册用户
  bool updateUser(int userId,bool death);  // 登录用户
  bool deleteUser(int userId);  // 注销用户
  int getUserId(const std::string& username); // 失败或不存在返回 -1
  std::string getSalt(int userId);  // 获取盐
  std::string getPwd(int userId);   // 获取哈希密码
 private:
  std::shared_ptr<MysqlConn> m_conn;
};

#endif //WEBDESK_SRC_MYSQL_USERDAO_H_
