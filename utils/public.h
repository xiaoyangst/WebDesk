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
#include <l8w8jwt/encode.h>
#include <l8w8jwt/decode.h>
namespace WebDesk {

// 定义颜色代码
#define RESET   "\033[0m"
#define RED     "\033[31m"      // 红色
#define GREEN   "\033[32m"      // 绿色
#define YELLOW  "\033[33m"      // 黄色
#define BLUE    "\033[34m"      // 蓝色


// 生成随机盐
std::string generate_salt(size_t length = 16);

// 计算密码哈希
std::string sha256_string(const std::string &input);

// 计算文件哈希
std::string getFileMD5(const std::string &filename);

// 有色打印
void printColor(const std::string &message, const std::string &color);

void JwtEncode(std::string& e_jwt,const std::string& aud);
bool JwtDecode(const std::string& d_jwt);

}

#endif //WEBDESK_UTILS_PUBLIC_H_
