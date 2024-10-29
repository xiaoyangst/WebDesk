cmake_minimum_required(VERSION 3.16)
project(WebDesk)

set(CMAKE_CXX_STANDARD 20)

link_directories(${PROJECT_SOURCE_DIR}/lib)

link_libraries(log4cpp)

include_directories(thirdpart/log4cpp/threading)
include_directories(thirdpart/log4cpp)
include_directories(thirdpart)
include_directories(utils)
include_directories(src)

aux_source_directory(thirdpart T_SRC)
aux_source_directory(utils U_SRC)
aux_source_directory(src S_SRC)

add_subdirectory(test)

add_executable(WebDesk main.cpp ${U_SRC} ${S_SRC} ${T_SRC})
target_link_libraries(WebDesk pthread)
