//
// Created by xy on 2024/11/7.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "public.h"
namespace WebDesk {
std::string getFileMD5(const std::string &filename) {
  MD5_CTX context;
  MD5Init(&context);

  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "无法打开文件: " << filename << std::endl;
    return "";
  }

  const size_t bufferSize = 1024 * 16; // 16 KB
  unsigned char buffer[bufferSize];

  while (file.good()) {
    file.read(reinterpret_cast<char *>(buffer), bufferSize);
    MD5Update(&context, buffer, file.gcount());
  }

  unsigned char digest[16];
  MD5Final(&context, digest);

  // 将 digest 转换为十六进制字符串
  std::string hash;
  for (int i = 0; i < 16; ++i) {
    char buf[3];
    snprintf(buf, sizeof(buf), "%02x", digest[i]);
    hash.append(buf);
  }

  return hash;
}

std::string sha256_string(const std::string &input) {
  std::vector<uint8_t> hash(SHA256_SIZE_BYTES);
  sha256(input.data(), input.size(), hash.data());

  std::stringstream ss;
  for (uint8_t byte : hash) {
    ss << std::setw(2) << std::setfill('0') << std::hex << (int) byte;
  }
  return ss.str();
}

std::string generate_salt(size_t length) {
  // 定义可选的字符集，包括大写字母、小写字母和数字
  const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  // 随机数生成器和分布
  std::random_device rd;  // 获取硬件随机数
  std::mt19937 gen(rd()); // 以硬件随机数为种子初始化Mersenne Twister引擎
  std::uniform_int_distribution<> dis(0, charset.size() - 1);

  std::string salt;
  for (size_t i = 0; i < length; ++i) {
    salt += charset[dis(gen)];  // 从字符集中随机选择一个字符
  }

  return salt;
}
}