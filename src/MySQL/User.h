//
// Created by xy on 2024/11/6.
//

#ifndef WEBDESK_SRC_MYSQL_USER_H_
#define WEBDESK_SRC_MYSQL_USER_H_

#include <string>
#include <optional>
#include <ctime>
#include <utility>
namespace WebDesk {
class User {
 public:
  int userId;
  std::string username;
  std::string password;
  std::string salt;
  bool death; // 0 表示存在该用户，1 表示不存在该用户（即注销）

  User() = default;
  User(int id, std::string user, std::string pass, std::string salt)
      : userId(id), username(std::move(user)), password(std::move(pass)), salt(std::move(salt)) {}

};
}
#endif //WEBDESK_SRC_MYSQL_USER_H_
