#include "UserDao.h"

#include <string>

UserDao::UserDao(std::shared_ptr<MysqlConn> &conn)
  :m_conn(conn)
{

}

bool
UserDao::addUser(const std::shared_ptr<User>& user) {  // 注册
  std::string query = "INSERT INTO users (username, password, salt, death) VALUES ('"
      + user->username + "', '" + user->password + "', '" + user->salt + "', "
      + std::to_string(user->death) + ")";
  std::cout << "Generated query: " << query << std::endl;
  return m_conn->update(query);
}

bool
UserDao::updateUser(int userId, bool death) { // 登录/离线后更改 death
  std::string query = "UPDATE users SET death=" + std::to_string(death)
      + " WHERE user_id=" + std::to_string(userId);
  return m_conn->update(query);
}


bool
UserDao::deleteUser(int userId) { // 注销
  std::string query = "DELETE FROM users WHERE user_id=" + std::to_string(userId);
  return m_conn->update(query);
}

