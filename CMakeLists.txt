cmake_minimum_required(VERSION 3.10)
project(chess-cli C)

link_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_executable(board bord.c)

