//
// Created by xy on 2024/10/28.
//


#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "Config.h"

TEST_CASE("A Config Test"){
  using namespace WebDesk::Utils;
  Config config;
  CHECK(config.getValue("server_host") == "127.0.0.1");
  CHECK(config.getValue("db_name") == "webdesk");
  CHECK(config.getValue("db_user") == "root");
  CHECK(config.getValue("db_pwd") == "root");
}
