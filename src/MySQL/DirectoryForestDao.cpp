#include "DirectoryForestDao.h"
DirectoryForestDao::DirectoryForestDao(std::shared_ptr<MysqlConn> &conn)
  :m_conn(conn)
{

}
int DirectoryForestDao::getFatId(const std::string &path) {
  std::string sql = "SELECT file_id FROM directory_forest WHERE file_path = '" + path + "'";

  if (m_conn->query(sql) && m_conn->next()) {
    return std::stoi(m_conn->value(0));
  }
  return -1; // 若查询失败，返回 -1 表示未找到
}
bool DirectoryForestDao::addDir(const DirectoryForest &dirForest) {
  // 设置 file_type 为 1 表示目录，file_hash 留空，其他字段采用默认值
  std::string sql = "INSERT INTO directory_forest (file_name, belong_user, father_id, file_path, file_type) VALUES ('"
      + dirForest.file_name + "', "
      + std::to_string(dirForest.belong_user) + ", "
      + std::to_string(dirForest.father_id) + ", '"
      + dirForest.file_path + "', 1)";

  return m_conn->update(sql);
}

