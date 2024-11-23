cmake_minimum_required(VERSION 3.16)
project(WebDesk)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(src)

add_executable(WebDesk main.cpp)
target_link_libraries(WebDesk pthread)
