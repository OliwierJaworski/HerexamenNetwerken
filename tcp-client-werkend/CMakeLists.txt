cmake_minimum_required(VERSION 3.23)
project(tcp_client C)

set(CMAKE_C_STANDARD 99)
link_libraries(ws2_32 wsock32)
add_executable(tcp_client ClientTCP.c)
