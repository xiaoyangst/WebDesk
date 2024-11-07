/**
  ******************************************************************************
  * @file           : public.h
  * @author         : xy
  * @brief          : 单独且可直接调用的函数
  * @attention      : None
  * @date           : 2024/11/6
  ******************************************************************************
  */

#ifndef WEBDESK_UTILS_PUBLIC_H_
#define WEBDESK_UTILS_PUBLIC_H_
#include <random>
#include "MD5.h"
#include "SHA256.h"
namespace WebDesk {
// 生成随机盐
std::string generate_salt(size_t length = 16);

// 计算密码哈希
std::string sha256_string(const std::string &input);

// 计算文件哈希
std::string getFileMD5(const std::string &filename);

}

#endif //WEBDESK_UTILS_PUBLIC_H_
