//
// Created by xy on 2024/11/6.
//

#ifndef WEBDESK_SRC_MYSQL_USER_H_
#define WEBDESK_SRC_MYSQL_USER_H_

#include <string>
#include <optional>
#include <ctime>
#include <utility>

class User {
 public:
  int userId;
  std::string username;
  std::string password;
  std::string salt;
  std::optional<std::time_t> lastActive;
  bool death;

  User() = default;
  User(int id, std::string user, std::string pass, std::string salt, bool death, std::optional<std::time_t> lastActive = std::nullopt)
      : userId(id), username(std::move(user)), password(std::move(pass)), salt(std::move(salt)), death(death), lastActive(lastActive) {}

};

#endif //WEBDESK_SRC_MYSQL_USER_H_
