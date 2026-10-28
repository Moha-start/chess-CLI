cmake_minimum_required(VERSION 3.10)
project(chess-cli C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

enable_testing()

# Création de la bibliothèque avec les fichiers dans src/board/
add_library(chess_core 
    src/board/bord.c 
    src/board/check.c
)

target_include_directories(chess_core PUBLIC src/board)

add_executable(board src/main.c)
target_link_libraries(board chess_core)

add_executable(test_board src/board/test_board.c)
target_link_libraries(test_board chess_core)

add_test(NAME Utilities COMMAND test_board utilities)
add_test(NAME Pawns COMMAND test_board pawns)
add_test(NAME Knights COMMAND test_board knights)
add_test(NAME Bishops COMMAND test_board bishops)
add_test(NAME Rooks COMMAND test_board rooks)
add_test(NAME Queens COMMAND test_board queens)
add_test(NAME Kings COMMAND test_board kings)