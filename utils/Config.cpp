
#include "Config.h"
#include "Log.h"

//using namespace std;

namespace WebDesk{

Config::Config(std::string path) : m_path(std::move(path)) {}

std::string
Config::getValue(const std::string &key) {
  std::ifstream m_file(m_path);
  if (!m_file.is_open()) {
    ERROR_LOG("Failed to open config file");
    return "";
  }

  std::string line;
  while (std::getline(m_file, line)) {
    size_t pos = isKey(line, key);
    if (pos != std::string::npos) {
      auto re = line.substr(pos + 1);
      return rtrim(re);
    }
  }
  return "";
}

size_t
Config::isKey(const std::string &line, const std::string &key) {
  size_t pos = line.find('=');
  if (pos != std::string::npos) {
    auto re = line.substr(0, pos);
    if (re == key) return pos;
  }
  return std::string::npos;
}

std::string
Config::rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(" \n\r\t");
  if (end != std::string::npos) {
    return s.substr(0, end + 1);
  }
  return ""; // 如果字符串全是空白，返回空字符串
}

}

