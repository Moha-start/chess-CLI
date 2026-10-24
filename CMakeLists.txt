cmake_minimum_required(VERSION 3.10)
project(chess-cli C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

# The core library/objects containing the logic
add_library(chess_core bord.c check.c)

# Main game executable
add_executable(board main.c)
target_link_libraries(board chess_core)

# Test executable
add_executable(test_board test_board.c)
target_link_libraries(test_board chess_core)