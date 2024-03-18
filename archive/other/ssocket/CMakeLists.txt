# 主要用于clion的cmake配置

cmake_minimum_required(VERSION 3.2)
project(ssocket)

include_directories(/usr/include)

add_executable(main src/main.c)

add_executable(client test/client.c)

TARGET_LINK_LIBRARIES(main pthread aio)
