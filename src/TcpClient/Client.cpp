//
// Created by xy on 2024/10/30.
//

#include "Config.h"
#include "CClient.h"

using namespace WebDesk;
Config c_config;

int main() {
  std::string server_ip = c_config.getValue("server_ip");
  int port = stoi(c_config.getValue("server_port"));



  CClient c_client(port,server_ip.c_str());

  return 0;
}




