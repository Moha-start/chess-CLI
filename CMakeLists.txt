cmake_minimum_required(VERSION 3.10)
project(chess-cli C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

enable_testing()

add_library(chess_core bord.c check.c)

add_executable(board main.c)
target_link_libraries(board chess_core)

add_executable(test_board test_board.c)
target_link_libraries(test_board chess_core)

# Register each suite individually
add_test(NAME Utilities COMMAND test_board utilities)
add_test(NAME Pawns COMMAND test_board pawns)
add_test(NAME Knights COMMAND test_board knights)
add_test(NAME Bishops COMMAND test_board bishops)
add_test(NAME Rooks COMMAND test_board rooks)
add_test(NAME Queens COMMAND test_board queens)
add_test(NAME Kings COMMAND test_board kings)