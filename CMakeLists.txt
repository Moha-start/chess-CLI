cmake_minimum_required(VERSION 3.10)
project(chess-cli C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")

enable_testing()

# ---------------------------------------------------------
# Stockfish Build Target
# ---------------------------------------------------------
include(ExternalProject)

set(STOCKFISH_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/stockfish/src")
#set(STOCKFISH_BIN "${CMAKE_CURRENT_SOURCE_DIR}/src/stockfish/stockfish")
set(STOCKFISH_BIN "${CMAKE_CURRENT_BINARY_DIR}/stockfish")
# Stockfish uses its own Makefile.
# We build using `make -C ... build ARCH=x86-64-modern` (or ARCH=native)
ExternalProject_Add(stockfish_build
    SOURCE_DIR "${STOCKFISH_SRC_DIR}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -C "${STOCKFISH_SRC_DIR}" build ARCH=x86-64
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy "${STOCKFISH_SRC_DIR}/stockfish" "${STOCKFISH_BIN}"
    BUILD_BYPRODUCTS "${STOCKFISH_BIN}"
)

# ---------------------------------------------------------
# Chess Core Library
# ---------------------------------------------------------
add_library(chess_core 
    src/board/bord.c 
    src/board/check.c
    src/board/mouvesStockfish.c
)

target_include_directories(chess_core PUBLIC src/board)

# ---------------------------------------------------------
# Executables
# ---------------------------------------------------------
add_executable(board src/main.c)
target_link_libraries(board chess_core)

# Ensure Stockfish binary is built before running the main binary
add_dependencies(board stockfish_build)

add_executable(test_board src/board/test_board.c)
target_link_libraries(test_board chess_core)

# ---------------------------------------------------------
# Tests
# ---------------------------------------------------------
add_test(NAME Utilities COMMAND test_board utilities)
add_test(NAME Pawns COMMAND test_board pawns)
add_test(NAME Knights COMMAND test_board knights)
add_test(NAME Bishops COMMAND test_board bishops)
add_test(NAME Rooks COMMAND test_board rooks)
add_test(NAME Queens COMMAND test_board queens)
add_test(NAME Kings COMMAND test_board kings)