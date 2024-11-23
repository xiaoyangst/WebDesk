//
// Created by xy on 2024/10/30.
//

#ifndef WEBDESK_SRC_HEAD_H_
#define WEBDESK_SRC_HEAD_H_

#include <string>




// 主界面
enum class MAINWINDOW{
  EXIT,REGISTER,LOGIN,LOGOUT,DEFAULT
};
// 网盘系统界面
enum class WEBDESK{
  PWD,LS,MKDIR,REMOVE,UPLOAD,DOWNLOAD,CD,QUIT,DEFAULT
};
// 信息
enum class MESSAGE{
  LOGINSUCCESS,LOGINFAIL,REGISTERSUCCESS,REGISTERFAIL,
  LOGOUTSUCCESS,LOGOUTFAIL,USERNOEXIST,PWDERROR,
  SUCCESS,FAIL,
  ADDDIRSUCCESS,ADDDIRFAIL,
  EXIST
};

// 主界面通信信息

// 网盘系统界面通信信息

// 用户个人的相关信息
struct UserInfo{
  std::string username;
  std::string password;
  std::string token;
  std::string current_path;
};

#endif //WEBDESK_SRC_HEAD_H_
