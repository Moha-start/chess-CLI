# Chess CLI

A small terminal-based chess engine written in C. The project focuses on representing a chess board, initializing the standard starting position, and validating whether a chosen move is legal for a given piece.

It is built as a CMake project and includes a simple interactive CLI as well as a unit-test-style executable for validating movement logic.

## Features

- Game state containing an 8x8 dynamically allocated board and a piece tracker
- Standard chess piece values for black and white pieces
- Board initialization for a full starting position
- Move validation for pawns, knights, bishops, rooks, queens, and kings
- Simple textual board rendering to the terminal
- CMake build setup with separate executables for the game and tests

## Project structure

- `main.c` – interactive CLI entry point
- `bord.h` / `bord.c` – board representation, initialization, printing, and basic move handling
- `check.h` / `check.c` – legal move validation logic
- `test_board.c` – test suite covering utility and piece-move checks
- `CMakeLists.txt` – build configuration

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

This creates:

- `build/board` – the interactive chess program
- `build/test_board` – test executable

## Run the game

```bash
./build/board
```

The application prompts for a move using start and end coordinates. The expected format is:

```text
A 2 B 4
```

This means:

- start square: A2
- end square: B4

The board uses file letters A–H and rank numbers 1–8. Input is read by `getMouve()` in `bord.c` and converted into internal board indices.

## Example

```text
$ ./build/board

    A   B   C   D   E   F   G   H
  +---+---+---+---+---+---+---+---+
8 | r | n | b | q | k | b | n | r | 8
7 | p | p | p | p | p | p | p | p | 7
6 | . | . | . | . | . | . | . | . | 6
5 | . | . | . | . | . | . | . | . | 5
4 | . | . | . | . | . | . | . | . | 4
3 | . | . | . | . | . | . | . | . | 3
2 | P | P | P | P | P | P | P | P | 2
1 | R | N | B | Q | K | B | N | R | 1
  +---+---+---+---+---+---+---+---+
    A   B   C   D   E   F   G   H
Enter values start end(x9x9): E 2 E 4
```

## Current status

This project is a chess logic prototype rather than a full chess game. It validates piece movement rules and board state, but it does not yet implement a complete chess engine with features such as:

- full legal-move checking for check/checkmate
- castling
- en passant
- turn management across an entire match
- move history and game state persistence

## Notes on the implementation

- The `GameState` structure in `bord.h` stores the board in its `grid` member and includes an `emptyPieces` tracker. Piece constants such as `W_KING`, `B_PAWN`, and `EMPTY` are defined there.
- `CheckMouve()` dispatches to piece-specific validation functions in `check.c`.
- `main.c` is a minimal CLI and is best treated as a demonstration harness for the move-validation logic.

## Running the tests

```bash
./build/test_board
```

The test executable exercises board setup and several legal/illegal move scenarios for the major pieces.
