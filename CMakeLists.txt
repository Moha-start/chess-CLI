cmake_minimum_required(VERSION 3.5)
project(chess-cli C)

link_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_executable(board bord.c)
//target_link_libraries(game_text game)