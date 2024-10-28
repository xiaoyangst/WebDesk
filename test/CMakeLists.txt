cmake_minimum_required(VERSION 3.16)
project(Test)

set(CMAKE_CXX_STANDARD 20)
aux_source_directory(../utils UT_SRC)

add_executable(Test TestUtils.cpp catch_amalgamated.cpp ${UT_SRC})