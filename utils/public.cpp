//
// Created by xy on 2024/11/7.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "public.h"
namespace WebDesk {

std::string
getFileMD5(const std::string &filename) {
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

std::string
sha256_string(const std::string &input) {
  std::vector<uint8_t> hash(SHA256_SIZE_BYTES);
  sha256(input.data(), input.size(), hash.data());

  std::stringstream ss;
  for (uint8_t byte : hash) {
    ss << std::setw(2) << std::setfill('0') << std::hex << (int) byte;
  }
  return ss.str();
}

std::string
generate_salt(size_t length) {
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

void
printColor(const std::string &message, const std::string &color) {
  std::cout << std::endl;
  std::cout << color << message << RESET << std::endl;
}

void JwtEncode(std::string &e_jwt,const std::string& aud) {
  // 用于存储生成的JWT字符串
  char *jwt;
  size_t jwt_length;

  struct l8w8jwt_encoding_params params;
  l8w8jwt_encoding_params_init(&params);

  params.alg = L8W8JWT_ALG_HS512;

  // 主题
  params.sub = "WebDesk";
  // 签发者
  params.iss = "xy";
  // 接收方
  params.aud = const_cast<char*>(aud.c_str());
  // token签发时间
  params.iat = time(NULL);
  // token过期/有效时间
  params.exp = time(NULL) + 600;  // 10 分钟后失效

  // 设置加密密钥
  params.secret_key = (unsigned char *) "xy string token key";
  params.secret_key_length = strlen((char *) params.secret_key);

  // 输出变量
  params.out = &jwt;
  params.out_length = &jwt_length;

  // 加密
  l8w8jwt_encode(&params);

  e_jwt = std::string(jwt);

  // 释放token 字符串的内存
  l8w8jwt_free(jwt);
}

bool JwtDecode(const std::string& d_jwt){
  // 初始化解码参数结构体
  struct l8w8jwt_decoding_params params;
  l8w8jwt_decoding_params_init(&params);

  // 设置JWT使用的算法，这里使用HS512
  params.alg = L8W8JWT_ALG_HS512;

  // 填入token字符串和长度
  params.jwt = const_cast<char*>(d_jwt.c_str());
  params.jwt_length = d_jwt.size();

  // 设置密钥
  char *key="xy string token key";
  params.verification_key = (unsigned char*)key;
  params.verification_key_length = strlen(key);

  // 定义: 接收结果的指针和变量
  struct l8w8jwt_claim *claims = NULL;
  size_t claim_count = 0;
  enum l8w8jwt_validation_result validation_result;

  // 解码: 获得token中存储的信息
  int decode_result = l8w8jwt_decode(&params, &validation_result, &claims, &claim_count);

  // 判断是否是正确的token
  if (decode_result == L8W8JWT_SUCCESS && validation_result == L8W8JWT_VALID) {
    l8w8jwt_free_claims(claims, claim_count);
    return true;
  } else {
    l8w8jwt_free_claims(claims, claim_count);
    return false;
  }


}
}