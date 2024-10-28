cmake_minimum_required(VERSION 3.16)
project(WebDesk)

set(CMAKE_CXX_STANDARD 20)

include_directories(thirdpart)
include_directories(utils)
include_directories(src)
aux_source_directory(thirdpart T_SRC)
aux_source_directory(utils U_SRC)
aux_source_directory(src S_SRC)

add_subdirectory(test)

#主程序
add_executable(WebDesk main.cpp ${U_SRC} ${S_SRC} ${T_SRC})

