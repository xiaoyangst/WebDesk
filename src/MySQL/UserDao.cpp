#include "UserDao.h"

#include <string>

UserDao::UserDao(std::shared_ptr<MysqlConn> &conn)
  :m_conn(conn)
{

}

bool
UserDao::addUser(const User& user) {  // 注册
  std::string query = "INSERT INTO users (username, password, salt) VALUES ('"
      + user.username + "', '" + user.password + "', '" + user.salt + "')";
  std::cout << "Generated query: " << query << std::endl;
  return m_conn->update(query);
}

bool
UserDao::updateUser(int userId, bool death) {
  std::string query = "UPDATE users SET death=" + std::to_string(death)
      + " WHERE user_id=" + std::to_string(userId);
  return m_conn->update(query);
}


bool
UserDao::deleteUser(int userId) {
  std::string query = "DELETE FROM users WHERE user_id=" + std::to_string(userId);
  return m_conn->update(query);
}

int
UserDao::getUserId(const std::string &username) {
  std::string query = "SELECT user_id FROM users WHERE username = '" + username + "'";
  if (!m_conn->query(query) || !m_conn->next()) {
    return -1;
  }
  int userId = std::stoi(m_conn->value(0));  // 获取第一个字段（user_id）
  return userId;
}

std::string
UserDao::getSalt(int userId) {
  std::string query = "SELECT salt FROM users WHERE user_id = " + std::to_string(userId);
  m_conn->query(query);
  m_conn->next();
  return m_conn->value(0);
}

std::string
UserDao::getPwd(int userId) {
  std::string query = "SELECT password FROM users WHERE user_id = " + std::to_string(userId);
  m_conn->query(query);
  m_conn->next();
  return m_conn->value(0);
}

