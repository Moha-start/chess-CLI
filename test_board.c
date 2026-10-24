#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "bord.h"
#include "check.h"

// Helper function to create an empty board for isolated piece testing
Board CreateEmptyTestBoard() {
    Board b = CreateNullBoard();
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            b[y][x] = EMPTY;
        }
    }
    return b;
}

void test_utilities() {
    assert(FromChartoUint('A') == 0);
    assert(FromChartoUint('h') == 7);
    assert(FromUintToChar(3) == 'D');
    
    assert(PieceToChar(B_KING) == 'k');
    assert(PieceToChar(W_QUEEN) == 'Q');
    assert(PieceToChar(EMPTY) == '.');
    
    printf("Utility function tests passed.\n");
}

void test_board_management() {
    Board b = CreateNullBoard();
    InitiliseBoard(b);
    
    // Check initial placement
    assert(b[0][0] == B_ROOK);
    assert(b[7][4] == W_KING);
    
    // Test check.c basic helpers
    assert(isEmpty(b, 4, 4) == true); // E4 should be empty initially
    assert(isEmpty(b, 0, 0) == false); // A8 (B_ROOK) is not empty
    
    assert(isWhite(b, 4, 7) == true); // E1 (W_KING) is white
    assert(isWhite(b, 4, 0) == false); // E8 (B_KING) is black
    
    FreeBoard(b);
    printf("Board management tests passed.\n");
}

void test_pawns() {
    Board b = CreateNullBoard();
    InitiliseBoard(b);
    
    // White Pawn valid moves (E2 to E3, E2 to E4)
    mouve wp_step = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 5, .isWhite = true };
    assert(CheckMouve(b, &wp_step) == true);
    
    mouve wp_double = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 4, .isWhite = true };
    assert(CheckMouve(b, &wp_double) == true);

    // Black Pawn valid moves (E7 to E6, E7 to E5)
    mouve bp_step = { .xStart = 4, .yStart = 1, .xEnd = 4, .yEnd = 2, .isWhite = false };
    assert(CheckMouve(b, &bp_step) == true);
    
    mouve bp_double = { .xStart = 4, .yStart = 1, .xEnd = 4, .yEnd = 3, .isWhite = false };
    assert(CheckMouve(b, &bp_double) == true);

    FreeBoard(b);
    printf("Pawn tests passed.\n");
}

void test_knights() {
    Board b = CreateEmptyTestBoard();
    b[4][4] = W_KNIGHT; // Place White Knight at E4 (x=4, y=4)

    // Valid L-shape move (E4 to F6 -> x=5, y=2)
    mouve k_valid = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 2, .isWhite = true };
    assert(CheckMouve(b, &k_valid) == true);

    // Invalid straight move
    mouve k_invalid = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 3, .isWhite = true };
    assert(CheckMouve(b, &k_invalid) == false);

    FreeBoard(b);
    printf("Knight tests passed.\n");
}

void test_bishops() {
    Board b = CreateEmptyTestBoard();
    b[4][4] = W_BISHOP; // E4

    // Valid diagonal (E4 to C6 -> x=2, y=2)
    mouve b_valid = { .xStart = 4, .yStart = 4, .xEnd = 2, .yEnd = 2, .isWhite = true };
    assert(CheckMouve(b, &b_valid) == true);

    // Invalid straight
    mouve b_invalid = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 2, .isWhite = true };
    assert(CheckMouve(b, &b_invalid) == false);

    FreeBoard(b);
    printf("Bishop tests passed.\n");
}

void test_rooks() {
    Board b = CreateEmptyTestBoard();
    b[4][4] = W_ROOK; // E4

    // Valid straight (E4 to E7 -> x=4, y=1)
    mouve r_valid = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 1, .isWhite = true };
    assert(CheckMouve(b, &r_valid) == true);

    // Invalid diagonal
    mouve r_invalid = { .xStart = 4, .yStart = 4, .xEnd = 6, .yEnd = 2, .isWhite = true };
    assert(CheckMouve(b, &r_invalid) == false);

    FreeBoard(b);
    printf("Rook tests passed.\n");
}

void test_queens() {
    Board b = CreateEmptyTestBoard();
    b[4][4] = W_QUEEN; // E4

    // Valid straight
    mouve q_valid_straight = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 1, .isWhite = true };
    assert(CheckMouve(b, &q_valid_straight) == true);

    // Valid diagonal
    mouve q_valid_diag = { .xStart = 4, .yStart = 4, .xEnd = 7, .yEnd = 1, .isWhite = true };
    assert(CheckMouve(b, &q_valid_diag) == true);

    // Invalid L-shape (Knight move)
    mouve q_invalid = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 2, .isWhite = true };
    assert(CheckMouve(b, &q_invalid) == false);

    FreeBoard(b);
    printf("Queen tests passed.\n");
}

void test_kings() {
    Board b = CreateEmptyTestBoard();
    b[4][4] = W_KING; // E4

    // Valid 1-step move
    mouve k_valid = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 5, .isWhite = true };
    assert(CheckMouve(b, &k_valid) == true);

    // Invalid 2-step move
    mouve k_invalid = { .xStart = 4, .yStart = 4, .xEnd = 6, .yEnd = 6, .isWhite = true };
    assert(CheckMouve(b, &k_invalid) == false);

    FreeBoard(b);
    printf("King tests passed.\n");
}

int main() {
    printf("Running unit tests...\n");
    
    test_utilities();
    test_board_management();
    test_pawns();
    
    // NOTE: The tests below will FAIL with an Assertion Error 
    // until you implement the correct logic in check.c
    test_knights();
    test_bishops();
    test_rooks();
    test_queens();
    test_kings();
    
    printf("All tests passed successfully!\n");
    return 0;
}