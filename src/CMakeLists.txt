include_directories(${PROJECT_SOURCE_DIR}/thirdpart/hv)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/log4cpp/threading)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/log4cpp)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart)
include_directories(${PROJECT_SOURCE_DIR}/utils)
include_directories(${PROJECT_SOURCE_DIR}/src)

include_directories(MySQL)
include_directories(.)

add_subdirectory(TcpClient)
add_subdirectory(TcpServer)
