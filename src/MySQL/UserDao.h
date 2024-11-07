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
  bool addUser(const std::shared_ptr<User>& user); // 注册用户
  bool updateUser(int userId,bool death);  // 登录用户
  bool deleteUser(int userId);  // 注销用户
 private:
  std::shared_ptr<MysqlConn> m_conn;
};

#endif //WEBDESK_SRC_MYSQL_USERDAO_H_
