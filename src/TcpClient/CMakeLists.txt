cmake_minimum_required(VERSION 3.16)
project(TcpClient)

set(CMAKE_CXX_STANDARD 20)

link_directories(${CMAKE_SOURCE_DIR}/lib)

link_libraries(log4cpp)
link_libraries(hv)

aux_source_directory(. TCPS_CRC)
aux_source_directory(${CMAKE_SOURCE_DIR}/utils TCPS_U_CRC)

add_executable(TcpClient Client.cpp ${TCPS_CRC} ${TCPS_U_CRC})
target_link_libraries(TcpClient pthread)