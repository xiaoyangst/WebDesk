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

  std::unique_ptr<CClient> c_client = std::make_unique<CClient>(port,server_ip);

  return 0;
}




