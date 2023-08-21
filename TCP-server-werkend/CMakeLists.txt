cmake_minimum_required(VERSION 3.23)
project(TCP_server C)

set(CMAKE_C_STANDARD 99)
link_libraries(ws2_32 wsock32)
add_executable(TCP_server ServerTCP.c)
