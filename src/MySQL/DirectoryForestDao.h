/**
  ******************************************************************************
  * @file           : DirectoryForestDao.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/21
  ******************************************************************************
  */

#ifndef WEBDESK_SRC_MYSQL_DIRECTORYFORESTDAO_H_
#define WEBDESK_SRC_MYSQL_DIRECTORYFORESTDAO_H_

#include <memory>
#include "DirectoryForest.h"
#include "MysqlConn.h"
using namespace WebDesk;
class DirectoryForestDao {
 public:
  explicit DirectoryForestDao(std::shared_ptr<MysqlConn> &conn);
  ~DirectoryForestDao() = default;

  // 删除文件夹(递归删除，利用 like )
  bool delDir(const std::string &path);
  // 删除文件
  bool delFile(int id);
  // 添加文件
  bool addFile(const std::string &userId, const DirectoryForest &dirForest);
  // 添加文件夹
  bool addDir(const DirectoryForest &dirForest);
  // 获取文件夹下所有文件和文件夹
  std::string lsDir(const std::string &userId, const DirectoryForest &dirForest);
  // 判断某个文件是否存在
  bool isExist(const std::string &userId, const DirectoryForest &dirForest);
  // 断点续传失败，修改对应状态
  bool changeTransfer(const std::string &userId, const DirectoryForest &dirForest);
  // 判断某个文件在当前用户下是否已经存在同名文件
  bool isSameName(const std::string &userId, const DirectoryForest &dirForest);
  // 判断某个文件是否标记位设置为删除
  bool isDel(const std::string &userId, const DirectoryForest &dirForest);

  // 获取ID
  int getId(const std::string &path);

  // 获取要删除文件的唯一的合适的记录的ID
  int getDelId(const std::string &path);
 private:
  std::shared_ptr<MysqlConn> m_conn;
};

#endif //WEBDESK_SRC_MYSQL_DIRECTORYFORESTDAO_H_
