//
// Created by xy on 2024/10/30.
//

#ifndef WEBDESK_SRC_HEAD_H_
#define WEBDESK_SRC_HEAD_H_

#include <string>

// 定义颜色代码
#define RESET   "\033[0m"
#define RED     "\033[31m"      // 红色
#define GREEN   "\033[32m"      // 绿色
#define YELLOW  "\033[33m"      // 黄色
#define BLUE    "\033[34m"      // 蓝色


// 主界面
enum class MAINWINDOW{
  EXIT,REGISTER,LOGIN,LOGOUT,DEFAULT
};
// 网盘系统界面
enum class WEBDESK{
  PWD,LS,MKDIR,REMOVE,UPLOAD,DOWNLOAD,QUIT,DEFAULT
};
// 信息
enum class MESSAGE{
  LOGINSUCCESS,LOGINFAIL,REGISTERSUCCESS,REGISTERFAIL,LOGOUTSUCCESS,LOGOUTFAIL,
  EXIST
};

// 主界面通信信息

// 网盘系统界面通信信息

// 用户个人的相关信息
struct UserInfo{
  std::string username;
  std::string password;
  std::string current_path;
};

#endif //WEBDESK_SRC_HEAD_H_
