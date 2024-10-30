cmake_minimum_required(VERSION 3.16)
project(TcpServer)

set(CMAKE_CXX_STANDARD 20)

link_directories(${CMAKE_SOURCE_DIR}/lib)

link_libraries(log4cpp)
link_libraries(hv)

aux_source_directory(. TCPS_SRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/utils TCPS_U_SRC)

add_executable(TcpServer Server.cpp ${TCPS_SRC} ${TCPS_U_SRC})
target_link_libraries(TcpServer pthread)