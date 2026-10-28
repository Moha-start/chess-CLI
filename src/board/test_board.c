#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bord.h"
#include "check.h"

int tests_run = 0;
int tests_failed = 0;

#define EXPECT_EQ(expected, actual, msg) \
    do { \
        tests_run++; \
        if ((expected) != (actual)) { \
            printf("[FAIL] %s\n", msg); \
            tests_failed++; \
        } \
    } while(0)

GameState *CreateEmptyTestBoard() {
    GameState *b = CreateNullBoard();
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            b->grid[y][x] = EMPTY;
        }
    }
    return b;
}

void test_utilities() {
    EXPECT_EQ(0, FromChartoUint('A'), "FromChartoUint('A') == 0");
    EXPECT_EQ(7, FromChartoUint('h'), "FromChartoUint('h') == 7");
    EXPECT_EQ('D', FromUintToChar(3,true), "FromUintToChar(3,true) == 'D'");
    EXPECT_EQ('k', PieceToChar(B_KING), "PieceToChar(B_KING) == 'k'");
    EXPECT_EQ('.', PieceToChar(EMPTY), "PieceToChar(EMPTY) == '.'");

    GameState *state = CreateNullBoard();
    for (int color = 0; color < 2; color++) {
        for (int piece = 0; piece < 13; piece++) {
            EXPECT_EQ(0, state->emptyPieces[color][piece], "Piece tracker initialized to zero");
        }
    }
    FreeBoard(state);
}

void test_pawns() {
    GameState *b = CreateNullBoard();
    InitiliseBoard(b);
    
    mouve wp_step = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 5, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &wp_step), "White Pawn single step forward");
    
    mouve wp_double = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 4, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &wp_double), "White Pawn double step from start");

    b->grid[5][4] = B_PAWN;
    EXPECT_EQ(false, CheckMouve(b, &wp_double), "White Pawn double step blocked by piece");

    FreeBoard(b);
}

void test_knights() {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_KNIGHT;

    mouve k_valid = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &k_valid), "White Knight valid L-shape jump");

    mouve k_invalid = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 3, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &k_invalid), "White Knight invalid straight move");

    b->grid[2][5] = W_PAWN;
    EXPECT_EQ(false, CheckMouve(b, &k_valid), "White Knight blocked by own piece on landing");

    FreeBoard(b);
}

void test_bishops() {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_BISHOP;

    mouve b_valid = { .xStart = 4, .yStart = 4, .xEnd = 2, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &b_valid), "White Bishop valid diagonal");

    mouve b_invalid = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &b_invalid), "White Bishop invalid straight");

    b->grid[3][3] = B_PAWN;
    EXPECT_EQ(false, CheckMouve(b, &b_valid), "White Bishop path blocked by enemy piece");

    FreeBoard(b);
}

void test_rooks() {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_ROOK;

    mouve r_valid = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 1, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &r_valid), "White Rook valid straight");

    mouve r_invalid = { .xStart = 4, .yStart = 4, .xEnd = 6, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &r_invalid), "White Rook invalid diagonal");

    b->grid[2][4] = W_PAWN;
    EXPECT_EQ(false, CheckMouve(b, &r_valid), "White Rook path blocked by friendly piece");

    FreeBoard(b);
}

void test_queens() {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_QUEEN;

    mouve q_valid_straight = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 1, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &q_valid_straight), "White Queen valid straight");

    mouve q_valid_diag = { .xStart = 4, .yStart = 4, .xEnd = 7, .yEnd = 1, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &q_valid_diag), "White Queen valid diagonal");

    mouve q_invalid = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &q_invalid), "White Queen invalid knight move");

    FreeBoard(b);
}

void test_kings() {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_KING;

    mouve k_valid = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 5, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &k_valid), "White King valid 1-step move");

    mouve k_invalid = { .xStart = 4, .yStart = 4, .xEnd = 6, .yEnd = 6, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &k_invalid), "White King invalid 2-step move");

    b->grid[4][7] = B_ROOK; // Attacks along y=4
    mouve k_into_check = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 4, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &k_into_check), "White King moving into check by Rook");

    FreeBoard(b);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Provide a test suite name (e.g., pawns, knights)\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "utilities") == 0) test_utilities();
    else if (strcmp(argv[1], "pawns") == 0) test_pawns();
    else if (strcmp(argv[1], "knights") == 0) test_knights();
    else if (strcmp(argv[1], "bishops") == 0) test_bishops();
    else if (strcmp(argv[1], "rooks") == 0) test_rooks();
    else if (strcmp(argv[1], "queens") == 0) test_queens();
    else if (strcmp(argv[1], "kings") == 0) test_kings();
    else {
        printf("Unknown test suite: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (tests_failed > 0) {
        printf("%d tests failed in suite '%s'\n", tests_failed, argv[1]);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}