//
// Created by xy on 2024/11/21.
//

#ifndef WEBDESK_SRC_MYSQL_DIRECTORYFOREST_H_
#define WEBDESK_SRC_MYSQL_DIRECTORYFOREST_H_

#include <string>
class DirectoryForest {
 public:
  int file_id;
  std::string file_name;
  int belong_user;
  int father_id;
  std::string file_path;
  int file_type;
  std::string file_hash;
  bool transfer_success;
  bool delete_sign;
};

#endif //WEBDESK_SRC_MYSQL_DIRECTORYFOREST_H_
