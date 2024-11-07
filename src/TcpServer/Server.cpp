//
// Created by xy on 2024/10/30.
//

#include <string>
#include "TcpServer.h"
#include "Config.h"
#include "SServer.h"

using namespace WebDesk;

Config s_config;

int main(){
  std::string server_ip = s_config.getValue("server_ip");
  int port = stoi(s_config.getValue("server_port"));

  SServer s(port,server_ip.c_str());

  return 0;
}

