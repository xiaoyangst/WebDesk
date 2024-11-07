/**
  ******************************************************************************
  * @file           : Config.h
  * @author         : xy
  * @brief          : 解析 ini 配置文件
  * @attention      : None
  * @date           : 2024/10/28
  ******************************************************************************
  */

#ifndef WEBDESK_UTILS_CONFIG_H_
#define WEBDESK_UTILS_CONFIG_H_

#include <string>
#include <fstream>
#include <iostream>

namespace WebDesk {

class Config {
 public:
  explicit Config(std::string path = "config.ini");
  ~Config() = default;
  std::string getValue(const std::string& key);
 private:
  // key 是否存在
  size_t isKey(const std::string& line,const std::string& key);
  std::string rtrim(const std::string &s);
 private:
  std::string m_path;
  std::ifstream m_file;
};

} // WebDesk::Utils

#endif //WEBDESK_UTILS_CONFIG_H_
