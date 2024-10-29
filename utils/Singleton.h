//
// Created by xy on 2024/10/28.
//

#ifndef WEBDESK_UTILS_SINGLETON_H_
#define WEBDESK_UTILS_SINGLETON_H_

#include <memory>
#include <mutex>
#include <iostream>


template <typename T>
class Singleton {
 protected:
  Singleton() = default;
  static std::shared_ptr<T> _instance;
 public:
  static std::shared_ptr<T> getInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() {
      _instance = std::shared_ptr<T>(new T);
    });
    return _instance;
  }
  void PrintAddress() {
    std::cout << _instance.get() << std::endl;
  }
  ~Singleton() {
    std::cout << "this is singleton destruct" << std::endl;
  }
  Singleton(const Singleton<T>&) = delete;
  Singleton& operator=(const Singleton<T>& st) = delete;
};
template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif //WEBDESK_UTILS_SINGLETON_H_
