#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "bord.h"
#include "check.h"
#include "mouvesStockfish.h"

/* Forward declarations for functions defined in .c files but omitted from headers */
void saveMouve(GameState *state, mouve *m);
bool isMoveLegal(GameState *state, mouve *mv);

int tests_run = 0;
int tests_failed = 0;

#define EXPECT_EQ(expected, actual, msg) \
    do { \
        tests_run++; \
        if ((expected) != (actual)) { \
            printf("[FAIL] %s (Expected: %ld, Actual: %ld)\n", msg, (long)(expected), (long)(actual)); \
            tests_failed++; \
        } \
    } while(0)

#define EXPECT_STR_EQ(expected, actual, msg) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) != 0) { \
            printf("[FAIL] %s (Expected: \"%s\", Actual: \"%s\")\n", msg, (expected), (actual)); \
            tests_failed++; \
        } \
    } while(0)

GameState *CreateEmptyTestBoard(void) {
    GameState *b = CreateNullBoard();
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            b->grid[y][x] = EMPTY;
        }
    }
    b->KingMouved[0] = false;
    b->KingMouved[1] = false;
    b->rookmouved[0][0] = false;
    b->rookmouved[0][1] = false;
    b->rookmouved[1][0] = false;
    b->rookmouved[1][1] = false;
    return b;
}

static void freeMoveList(mouve **list, uint count) {
    for (uint i = 0; i < count; i++) {
        free(list[i]);
    }
}

/* =========================================================================
 * BORD.C FUNCTION TESTS
 * ========================================================================= */

void test_CreateNullBoard(void) {
    GameState *b = CreateNullBoard();
    EXPECT_EQ(true, b != NULL, "CreateNullBoard: returns non-NULL state");
    EXPECT_EQ(true, b->grid != NULL, "CreateNullBoard: allocates grid");
    EXPECT_EQ(true, b->oldMouves != NULL, "CreateNullBoard: allocates oldMouves");
    EXPECT_EQ(0, b->Indexmouve, "CreateNullBoard: Indexmouve initialized to 0");
    EXPECT_EQ(false, b->hasLastMove, "CreateNullBoard: hasLastMove initialized to false");
    EXPECT_EQ('\0', b->oldMouves[0], "CreateNullBoard: oldMouves starts as empty string");

    for (int c = 0; c < 2; c++) {
        for (int p = 0; p < 13; p++) {
            EXPECT_EQ(0, b->emptyPieces[c][p], "CreateNullBoard: emptyPieces zeroed");
        }
    }
    FreeBoard(b);
}

void test_FreeBoard(void) {
    /* FreeBoard(NULL) should safely return without crashing */
    FreeBoard(NULL);
    GameState *b = CreateNullBoard();
    FreeBoard(b);
    EXPECT_EQ(1, 1, "FreeBoard: handles NULL and allocated GameState safely");
}

void test_combineStr(void) {
    char *res1 = combineStr("position startpos moves ", "e2e4 e7e5 ", 24);
    EXPECT_STR_EQ("position startpos moves e2e4 e7e5 \n", res1, "combineStr: full prefix + moves");
    free(res1);

    char *res2 = combineStr("abcdef", "XYZ", 3);
    EXPECT_STR_EQ("abcXYZ\n", res2, "combineStr: truncated prefix at index 3");
    free(res2);

    char *res3 = combineStr("hi", "there", 100);
    EXPECT_STR_EQ("hithere\n", res3, "combineStr: index larger than strlen clamps to length");
    free(res3);
}

void test_InitiliseBoard(void) {
    GameState *b = CreateNullBoard();
    InitiliseBoard(b);

    EXPECT_EQ(B_ROOK, b->grid[0][0], "InitiliseBoard: a8 is Black Rook");
    EXPECT_EQ(B_KNIGHT, b->grid[0][1], "InitiliseBoard: b8 is Black Knight");
    EXPECT_EQ(B_BISHOP, b->grid[0][2], "InitiliseBoard: c8 is Black Bishop");
    EXPECT_EQ(B_QUEEN, b->grid[0][3], "InitiliseBoard: d8 is Black Queen");
    EXPECT_EQ(B_KING, b->grid[0][4], "InitiliseBoard: e8 is Black King");
    EXPECT_EQ(B_PAWN, b->grid[1][3], "InitiliseBoard: rank 7 has Black Pawns");
    EXPECT_EQ(EMPTY, b->grid[4][4], "InitiliseBoard: center squares are EMPTY");
    EXPECT_EQ(W_PAWN, b->grid[6][4], "InitiliseBoard: rank 2 has White Pawns");
    EXPECT_EQ(W_ROOK, b->grid[7][7], "InitiliseBoard: h1 is White Rook");
    EXPECT_EQ(W_QUEEN, b->grid[7][3], "InitiliseBoard: d1 is White Queen");
    EXPECT_EQ(W_KING, b->grid[7][4], "InitiliseBoard: e1 is White King");

    EXPECT_EQ(false, b->KingMouved[0], "InitiliseBoard: White KingMouved is false");
    EXPECT_EQ(false, b->KingMouved[1], "InitiliseBoard: Black KingMouved is false");
    EXPECT_EQ(false, b->rookmouved[0][0], "InitiliseBoard: White rook[0] is false");
    EXPECT_EQ(false, b->rookmouved[1][1], "InitiliseBoard: Black rook[1] is false");

    FreeBoard(b);
}

void test_PieceToChar(void) {
    EXPECT_EQ('.', PieceToChar(EMPTY), "PieceToChar: EMPTY -> '.'");
    EXPECT_EQ('p', PieceToChar(B_PAWN), "PieceToChar: B_PAWN -> 'p'");
    EXPECT_EQ('n', PieceToChar(B_KNIGHT), "PieceToChar: B_KNIGHT -> 'n'");
    EXPECT_EQ('b', PieceToChar(B_BISHOP), "PieceToChar: B_BISHOP -> 'b'");
    EXPECT_EQ('r', PieceToChar(B_ROOK), "PieceToChar: B_ROOK -> 'r'");
    EXPECT_EQ('q', PieceToChar(B_QUEEN), "PieceToChar: B_QUEEN -> 'q'");
    EXPECT_EQ('k', PieceToChar(B_KING), "PieceToChar: B_KING -> 'k'");
    EXPECT_EQ('P', PieceToChar(W_PAWN), "PieceToChar: W_PAWN -> 'P'");
    EXPECT_EQ('N', PieceToChar(W_KNIGHT), "PieceToChar: W_KNIGHT -> 'N'");
    EXPECT_EQ('B', PieceToChar(W_BISHOP), "PieceToChar: W_BISHOP -> 'B'");
    EXPECT_EQ('R', PieceToChar(W_ROOK), "PieceToChar: W_ROOK -> 'R'");
    EXPECT_EQ('Q', PieceToChar(W_QUEEN), "PieceToChar: W_QUEEN -> 'Q'");
    EXPECT_EQ('K', PieceToChar(W_KING), "PieceToChar: W_KING -> 'K'");
    EXPECT_EQ('.', PieceToChar(999), "PieceToChar: invalid ID -> '.'");
}

void test_FromChartoUint(void) {
    EXPECT_EQ(0, FromChartoUint('A'), "FromChartoUint: 'A' -> 0");
    EXPECT_EQ(0, FromChartoUint('a'), "FromChartoUint: 'a' -> 0");
    EXPECT_EQ(3, FromChartoUint('D'), "FromChartoUint: 'D' -> 3");
    EXPECT_EQ(7, FromChartoUint('H'), "FromChartoUint: 'H' -> 7");
    EXPECT_EQ(7, FromChartoUint('h'), "FromChartoUint: 'h' -> 7");
    EXPECT_EQ(99, FromChartoUint('q'), "FromChartoUint: 'q' -> 99 (quit)");
    EXPECT_EQ(99, FromChartoUint('Q'), "FromChartoUint: 'Q' -> 99 (quit)");
    EXPECT_EQ(UINT_MAX, FromChartoUint('Z'), "FromChartoUint: 'Z' -> UINT_MAX");
}

void test_FromUintToChar(void) {
    EXPECT_EQ('A', FromUintToChar(0, true), "FromUintToChar: (0, true) -> 'A'");
    EXPECT_EQ('a', FromUintToChar(0, false), "FromUintToChar: (0, false) -> 'a'");
    EXPECT_EQ('H', FromUintToChar(7, true), "FromUintToChar: (7, true) -> 'H'");
    EXPECT_EQ('h', FromUintToChar(7, false), "FromUintToChar: (7, false) -> 'h'");
    EXPECT_EQ('\0', FromUintToChar(8, true), "FromUintToChar: out of bounds (8) -> '\\0'");
}

void test_saveMouve(void) {
    GameState *b = CreateEmptyTestBoard();
    mouve m1 = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 4, .isWhite = true }; /* e2e4 */
    mouve m2 = { .xStart = 4, .yStart = 1, .xEnd = 4, .yEnd = 3, .isWhite = false }; /* e7e5 */

    saveMouve(b, &m1);
    EXPECT_EQ(5, b->Indexmouve, "saveMouve: Indexmouve incremented to 5");
    EXPECT_STR_EQ("e2e4 ", b->oldMouves, "saveMouve: records 'e2e4 '");

    saveMouve(b, &m2);
    EXPECT_EQ(10, b->Indexmouve, "saveMouve: Indexmouve incremented to 10");
    EXPECT_STR_EQ("e2e4 e7e5 ", b->oldMouves, "saveMouve: appends 'e7e5 '");

    FreeBoard(b);
}

void test_Mouve(void) {
    GameState *b = CreateNullBoard();
    InitiliseBoard(b);

    /* 1. Standard valid move: e2 -> e4 */
    mouve m1 = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 4, .isWhite = true };
    EXPECT_EQ(true, Mouve(b, &m1), "Mouve: e2e4 succeeds");
    EXPECT_EQ(W_PAWN, b->grid[4][4], "Mouve: W_PAWN placed on e4");
    EXPECT_EQ(EMPTY, b->grid[6][4], "Mouve: e2 is now EMPTY");
    EXPECT_EQ(true, b->hasLastMove, "Mouve: hasLastMove set to true");
    EXPECT_STR_EQ("e2e4 ", b->oldMouves, "Mouve: history updated");

    /* 2. Invalid move rejected */
    mouve m_bad = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(false, Mouve(b, &m_bad), "Mouve: invalid double step from e4 rejected");

    /* 3. Capture tracking & En Passant execution */
    GameState *ep = CreateEmptyTestBoard();
    ep->grid[3][4] = W_PAWN; /* White pawn on e5 (y=3, x=4) */
    ep->grid[1][3] = B_PAWN; /* Black pawn on d7 (y=1, x=3) */
    mouve b_double = { .xStart = 3, .yStart = 1, .xEnd = 3, .yEnd = 3, .isWhite = false };
    EXPECT_EQ(true, Mouve(ep, &b_double), "Mouve: Black d7d5 double step");

    mouve w_ep = { .xStart = 4, .yStart = 3, .xEnd = 3, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(true, Mouve(ep, &w_ep), "Mouve: White e5xd6 en passant succeeds");
    EXPECT_EQ(W_PAWN, ep->grid[2][3], "Mouve: White pawn lands on d6");
    EXPECT_EQ(EMPTY, ep->grid[3][3], "Mouve: Captured Black pawn on d5 removed");
    EXPECT_EQ(1, ep->emptyPieces[1][PAWN], "Mouve: Black pawn capture counter incremented");

    /* 4. King & Rook movement flags */
    ep->grid[7][4] = W_KING;
    ep->grid[7][7] = W_ROOK;
    mouve k_mv = { .xStart = 4, .yStart = 7, .xEnd = 4, .yEnd = 6, .isWhite = true };
    Mouve(ep, &k_mv);
    EXPECT_EQ(true, ep->KingMouved[0], "Mouve: White KingMouved flag updated");

    mouve r_mv = { .xStart = 7, .yStart = 7, .xEnd = 7, .yEnd = 6, .isWhite = true };
    Mouve(ep, &r_mv);
    EXPECT_EQ(true, ep->rookmouved[0][1], "Mouve: White h1 rookmouved flag updated");

    FreeBoard(b);
    FreeBoard(ep);
}

/* =========================================================================
 * CHECK.C FUNCTION TESTS
 * ========================================================================= */

void test_getRank(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[3][4] = W_QUEEN;
    b->grid[1][2] = B_KNIGHT;
    EXPECT_EQ(QUEEN, getRank(b, 4, 3), "getRank: W_QUEEN -> QUEEN");
    EXPECT_EQ(KNIGHT, getRank(b, 2, 1), "getRank: B_KNIGHT -> KNIGHT");
    EXPECT_EQ(EMPTY, getRank(b, 0, 0), "getRank: EMPTY -> 0");
    FreeBoard(b);
}

void test_getRankPiece(void) {
    EXPECT_EQ(PAWN, getRankPiece(W_PAWN), "getRankPiece: W_PAWN -> PAWN");
    EXPECT_EQ(KING, getRankPiece(B_KING), "getRankPiece: B_KING -> KING");
    EXPECT_EQ(ROOK, getRankPiece(B_ROOK), "getRankPiece: B_ROOK -> ROOK");
    EXPECT_EQ(EMPTY, getRankPiece(EMPTY), "getRankPiece: EMPTY -> 0");
}

void test_isEmpty(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[0][0] = B_ROOK;
    EXPECT_EQ(true, isEmpty(b, 4, 4), "isEmpty: empty square returns true");
    EXPECT_EQ(false, isEmpty(b, 0, 0), "isEmpty: occupied square returns false");
    FreeBoard(b);
}

void test_isEmptyPiece(void) {
    EXPECT_EQ(true, isEmptyPiece(EMPTY), "isEmptyPiece: EMPTY returns true");
    EXPECT_EQ(false, isEmptyPiece(W_PAWN), "isEmptyPiece: W_PAWN returns false");
}

void test_isWhite(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[0][0] = W_BISHOP;
    b->grid[0][1] = B_BISHOP;
    EXPECT_EQ(true, isWhite(b, 0, 0), "isWhite: White piece returns true");
    EXPECT_EQ(false, isWhite(b, 1, 0), "isWhite: Black piece returns false");
    FreeBoard(b);
}

void test_isWhitePiece(void) {
    EXPECT_EQ(true, isWhitePiece(W_KING), "isWhitePiece: W_KING returns true");
    EXPECT_EQ(false, isWhitePiece(B_KING), "isWhitePiece: B_KING returns false");
}

void test_cordAllPieces(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[6][4] = W_PAWN;
    b->grid[7][4] = W_KING;
    b->grid[0][4] = B_KING;

    uint **w_cords = cordAllPieces(b, true);
    EXPECT_EQ(6, w_cords[0][0], "cordAllPieces: 1st White piece row is 6");
    EXPECT_EQ(4, w_cords[0][1], "cordAllPieces: 1st White piece col is 4");
    EXPECT_EQ(7, w_cords[1][0], "cordAllPieces: 2nd White piece row is 7");
    EXPECT_EQ(4, w_cords[1][1], "cordAllPieces: 2nd White piece col is 4");
    EXPECT_EQ(99, w_cords[2][0], "cordAllPieces: unused slot row is 99");

    for (int i = 0; i < 16; i++) free(w_cords[i]);
    free(w_cords);
    FreeBoard(b);
}

void test_getKings(void) {
    GameState *b = CreateEmptyTestBoard();
    /* Note: getKings iterates i (x) and j (y) via getRank(state, i, j) */
    b->grid[7][4] = W_KING; /* x=4, y=7 */
    b->grid[0][3] = B_KING; /* x=3, y=0 */

    uint row0[2], row1[2];
    uint *coord[2] = {row0, row1};
    getKings(b, coord);

    EXPECT_EQ(4, coord[0][0], "getKings: White King x == 4");
    EXPECT_EQ(7, coord[0][1], "getKings: White King y == 7");
    EXPECT_EQ(3, coord[1][0], "getKings: Black King x == 3");
    EXPECT_EQ(0, coord[1][1], "getKings: Black King y == 0");

    FreeBoard(b);
}

void test_isKingAttaked(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_KING;

    EXPECT_EQ(false, isKingAttaked(b, 4, 4, true), "isKingAttaked: safe king returns false");

    /* Attacked by Black Rook along file */
    b->grid[0][4] = B_ROOK;
    EXPECT_EQ(true, isKingAttaked(b, 4, 4, true), "isKingAttaked: attacked by Rook on file");
    b->grid[0][4] = EMPTY;

    /* Attacked by Black Bishop on diagonal */
    b->grid[1][1] = B_BISHOP;
    EXPECT_EQ(true, isKingAttaked(b, 4, 4, true), "isKingAttaked: attacked by Bishop on diagonal");
    b->grid[1][1] = EMPTY;

    /* Attacked by Black Knight */
    b->grid[2][5] = B_KNIGHT;
    EXPECT_EQ(true, isKingAttaked(b, 4, 4, true), "isKingAttaked: attacked by Knight");
    b->grid[2][5] = EMPTY;

    /* Attacked by Black Pawn at (3, 3) -> above-left of White King at (4, 4) */
    b->grid[3][3] = B_PAWN;
    EXPECT_EQ(true, isKingAttaked(b, 4, 4, true), "isKingAttaked: attacked by Black Pawn");
    b->grid[3][3] = EMPTY;

    FreeBoard(b);
}

void test_isMoveLegal(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[7][4] = W_KING;
    b->grid[0][3] = B_KING;
    b->grid[6][4] = W_ROOK; /* Pinned White Rook on e2 */
    b->grid[0][4] = B_ROOK; /* Attacking Black Rook on e8 */

    /* Moving pinned rook off the e-file exposes White King to check */
    mouve illegal_pin = { .xStart = 4, .yStart = 6, .xEnd = 0, .yEnd = 6, .isWhite = true };
    EXPECT_EQ(false, isMoveLegal(b, &illegal_pin), "isMoveLegal: pinned Rook cannot leave file");

    /* Moving pinned rook along the e-file keeps King protected */
    mouve legal_slide = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 3, .isWhite = true };
    EXPECT_EQ(true, isMoveLegal(b, &legal_slide), "isMoveLegal: pinned Rook can slide along attack line");

    FreeBoard(b);
}

void test_isEnPassant(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[3][4] = W_PAWN; /* White on e5 */
    b->grid[3][3] = B_PAWN; /* Black on d5 */
    b->hasLastMove = true;
    b->lastMovePlayed = (mouve){ .xStart = 3, .yStart = 1, .xEnd = 3, .yEnd = 3, .isWhite = false };

    mouve ep_valid = { .xStart = 4, .yStart = 3, .xEnd = 3, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(true, isEnPassant(b, &ep_valid), "isEnPassant: valid after opponent double-step");

    /* If last move was only 1 square, en passant is false */
    b->lastMovePlayed.yStart = 2;
    EXPECT_EQ(false, isEnPassant(b, &ep_valid), "isEnPassant: false when last move was single-step");

    FreeBoard(b);
}

void test_iscastling(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[7][4] = W_KING;
    b->grid[7][7] = W_ROOK;

    mouve castle_mv = { .xStart = 4, .yStart = 7, .xEnd = 7, .yEnd = 7, .isWhite = true };
    b->KingMouved[0] = true;
    b->rookmouved[0][1] = true;
    EXPECT_EQ(true, iscastling(b, &castle_mv), "iscastling: returns true when conditions in iscastling met");

    b->grid[7][7] = EMPTY;
    EXPECT_EQ(false, iscastling(b, &castle_mv), "iscastling: returns false when destination has no rook");

    FreeBoard(b);
}

void test_CheckWhitePawn(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[6][4] = W_PAWN;

    mouve step1 = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 5, .isWhite = true };
    mouve step2 = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 4, .isWhite = true };
    mouve backward = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 7, .isWhite = true };
    mouve diag_empty = { .xStart = 4, .yStart = 6, .xEnd = 5, .yEnd = 5, .isWhite = true };

    EXPECT_EQ(true, CheckWhitePawn(b, &step1), "CheckWhitePawn: 1 step forward");
    EXPECT_EQ(true, CheckWhitePawn(b, &step2), "CheckWhitePawn: 2 steps from start rank");
    EXPECT_EQ(false, CheckWhitePawn(b, &backward), "CheckWhitePawn: backward move rejected");
    EXPECT_EQ(false, CheckWhitePawn(b, &diag_empty), "CheckWhitePawn: diagonal into empty rejected");

    b->grid[5][5] = B_PAWN;
    EXPECT_EQ(true, CheckWhitePawn(b, &diag_empty), "CheckWhitePawn: diagonal capture of Black piece");

    FreeBoard(b);
}

void test_CheckBlackPawn(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[1][4] = B_PAWN;

    mouve step1 = { .xStart = 4, .yStart = 1, .xEnd = 4, .yEnd = 2, .isWhite = false };
    mouve step2 = { .xStart = 4, .yStart = 1, .xEnd = 4, .yEnd = 3, .isWhite = false };
    mouve backward = { .xStart = 4, .yStart = 1, .xEnd = 4, .yEnd = 0, .isWhite = false };
    mouve diag = { .xStart = 4, .yStart = 1, .xEnd = 3, .yEnd = 2, .isWhite = false };

    EXPECT_EQ(true, CheckBlackPawn(b, &step1), "CheckBlackPawn: 1 step forward");
    EXPECT_EQ(true, CheckBlackPawn(b, &step2), "CheckBlackPawn: 2 steps from start rank");
    EXPECT_EQ(false, CheckBlackPawn(b, &backward), "CheckBlackPawn: backward move rejected");
    EXPECT_EQ(false, CheckBlackPawn(b, &diag), "CheckBlackPawn: diagonal into empty rejected");

    b->grid[2][3] = W_PAWN;
    EXPECT_EQ(true, CheckBlackPawn(b, &diag), "CheckBlackPawn: diagonal capture of White piece");

    FreeBoard(b);
}

void test_CheckWhiteRook(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_ROOK;

    mouve vert = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 0, .isWhite = true };
    mouve horiz = { .xStart = 4, .yStart = 4, .xEnd = 7, .yEnd = 4, .isWhite = true };
    mouve diag = { .xStart = 4, .yStart = 4, .xEnd = 6, .yEnd = 6, .isWhite = true };

    EXPECT_EQ(true, CheckWhiteRook(b, &vert), "CheckWhiteRook: clear vertical move");
    EXPECT_EQ(true, CheckWhiteRook(b, &horiz), "CheckWhiteRook: clear horizontal move");
    EXPECT_EQ(false, CheckWhiteRook(b, &diag), "CheckWhiteRook: diagonal move rejected");

    b->grid[2][4] = B_PAWN;
    EXPECT_EQ(false, CheckWhiteRook(b, &vert), "CheckWhiteRook: blocked vertical path rejected");

    FreeBoard(b);
}

void test_CheckBlackRook(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = B_ROOK;

    mouve vert = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 7, .isWhite = false };
    EXPECT_EQ(true, CheckBlackRook(b, &vert), "CheckBlackRook: clear vertical move");

    b->grid[7][4] = B_PAWN;
    EXPECT_EQ(false, CheckBlackRook(b, &vert), "CheckBlackRook: cannot capture own Black piece");

    b->grid[7][4] = W_PAWN;
    EXPECT_EQ(true, CheckBlackRook(b, &vert), "CheckBlackRook: can capture opponent White piece");

    FreeBoard(b);
}

void test_CheckWhiteBishop(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_BISHOP;

    mouve diag = { .xStart = 4, .yStart = 4, .xEnd = 1, .yEnd = 1, .isWhite = true };
    mouve straight = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 1, .isWhite = true };

    EXPECT_EQ(true, CheckWhiteBishop(b, &diag), "CheckWhiteBishop: clear diagonal move");
    EXPECT_EQ(false, CheckWhiteBishop(b, &straight), "CheckWhiteBishop: straight move rejected");

    b->grid[2][2] = W_PAWN;
    EXPECT_EQ(false, CheckWhiteBishop(b, &diag), "CheckWhiteBishop: blocked diagonal rejected");

    FreeBoard(b);
}

void test_CheckBlackBishop(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = B_BISHOP;

    mouve diag = { .xStart = 4, .yStart = 4, .xEnd = 7, .yEnd = 7, .isWhite = false };
    EXPECT_EQ(true, CheckBlackBishop(b, &diag), "CheckBlackBishop: clear diagonal move");

    b->grid[7][7] = B_PAWN;
    EXPECT_EQ(false, CheckBlackBishop(b, &diag), "CheckBlackBishop: cannot capture own Black piece");

    b->grid[7][7] = W_PAWN;
    EXPECT_EQ(true, CheckBlackBishop(b, &diag), "CheckBlackBishop: can capture White piece");

    FreeBoard(b);
}

void test_CheckWhiteKnight(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_KNIGHT;

    mouve l_jump = { .xStart = 4, .yStart = 4, .xEnd = 6, .yEnd = 5, .isWhite = true };
    mouve bad = { .xStart = 4, .yStart = 4, .xEnd = 6, .yEnd = 6, .isWhite = true };

    EXPECT_EQ(true, CheckWhiteKnight(b, &l_jump), "CheckWhiteKnight: valid L-jump");
    EXPECT_EQ(false, CheckWhiteKnight(b, &bad), "CheckWhiteKnight: non-L move rejected");

    b->grid[5][6] = W_PAWN;
    EXPECT_EQ(false, CheckWhiteKnight(b, &l_jump), "CheckWhiteKnight: landing on friendly rejected");

    FreeBoard(b);
}

void test_CheckBlackKnight(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = B_KNIGHT;

    mouve l_jump = { .xStart = 4, .yStart = 4, .xEnd = 2, .yEnd = 3, .isWhite = false };
    EXPECT_EQ(true, CheckBlackKnight(b, &l_jump), "CheckBlackKnight: valid L-jump");

    b->grid[3][2] = B_PAWN;
    EXPECT_EQ(false, CheckBlackKnight(b, &l_jump), "CheckBlackKnight: landing on Black piece rejected");

    b->grid[3][2] = W_PAWN;
    EXPECT_EQ(true, CheckBlackKnight(b, &l_jump), "CheckBlackKnight: capturing White piece accepted");

    FreeBoard(b);
}

void test_CheckWhiteQueen(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_QUEEN;

    mouve straight = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 0, .isWhite = true };
    mouve diag = { .xStart = 4, .yStart = 4, .xEnd = 7, .yEnd = 7, .isWhite = true };
    mouve knight_like = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 2, .isWhite = true };

    EXPECT_EQ(true, CheckWhiteQueen(b, &straight), "CheckWhiteQueen: valid straight move");
    EXPECT_EQ(true, CheckWhiteQueen(b, &diag), "CheckWhiteQueen: valid diagonal move");
    EXPECT_EQ(false, CheckWhiteQueen(b, &knight_like), "CheckWhiteQueen: knight jump rejected");

    FreeBoard(b);
}

void test_CheckBlackQueen(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = B_QUEEN;

    mouve straight = { .xStart = 4, .yStart = 4, .xEnd = 0, .yEnd = 4, .isWhite = false };
    mouve diag = { .xStart = 4, .yStart = 4, .xEnd = 1, .yEnd = 7, .isWhite = false };

    EXPECT_EQ(true, CheckBlackQueen(b, &straight), "CheckBlackQueen: valid straight move");
    EXPECT_EQ(true, CheckBlackQueen(b, &diag), "CheckBlackQueen: valid diagonal move");

    b->grid[7][1] = B_PAWN;
    EXPECT_EQ(false, CheckBlackQueen(b, &diag), "CheckBlackQueen: blocked by friendly on target");

    FreeBoard(b);
}

void test_CheckWhiteKing(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_KING;

    mouve step = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 3, .isWhite = true };
    mouve far = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 2, .isWhite = true };

    EXPECT_EQ(true, CheckWhiteKing(b, &step), "CheckWhiteKing: valid 1-square step");
    EXPECT_EQ(false, CheckWhiteKing(b, &far), "CheckWhiteKing: 2-square step rejected");

    b->grid[3][0] = B_ROOK; /* Attacks row y=3 */
    EXPECT_EQ(false, CheckWhiteKing(b, &step), "CheckWhiteKing: stepping into check rejected");

    FreeBoard(b);
}

void test_CheckBlackKing(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = B_KING;

    mouve step = { .xStart = 4, .yStart = 4, .xEnd = 5, .yEnd = 5, .isWhite = false };
    EXPECT_EQ(true, CheckBlackKing(b, &step), "CheckBlackKing: valid 1-square diagonal step");

    b->grid[7][5] = W_ROOK; /* Attacks col x=5 */
    EXPECT_EQ(false, CheckBlackKing(b, &step), "CheckBlackKing: stepping into check rejected");

    FreeBoard(b);
}

void test_CheckMouve(void) {
    GameState *b = CreateNullBoard();
    InitiliseBoard(b);

    mouve empty_sq = { .xStart = 4, .yStart = 4, .xEnd = 4, .yEnd = 3, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &empty_sq), "CheckMouve: moving from empty square rejected");

    mouve wrong_turn = { .xStart = 4, .yStart = 1, .xEnd = 4, .yEnd = 2, .isWhite = true };
    EXPECT_EQ(false, CheckMouve(b, &wrong_turn), "CheckMouve: White moving Black piece rejected");

    mouve valid_wp = { .xStart = 4, .yStart = 6, .xEnd = 4, .yEnd = 4, .isWhite = true };
    EXPECT_EQ(true, CheckMouve(b, &valid_wp), "CheckMouve: valid White pawn dispatch");

    FreeBoard(b);
}

void test_allValideMouvesPawn(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[7][4] = W_KING;
    b->grid[0][4] = B_KING;
    b->grid[6][0] = W_PAWN; /* a2: can move to a3 or a4 */

    mouve *list[32];
    uint count = 0;
    allValideMouvesPawn(b, true, 0, 6, list, &count);
    EXPECT_EQ(2, count, "allValideMouvesPawn: unblocked start pawn has 2 moves");
    freeMoveList(list, count);

    FreeBoard(b);
}

void test_allValideMouvesKnight(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[7][4] = W_KING;
    b->grid[0][4] = B_KING;
    b->grid[4][4] = W_KNIGHT; /* Center knight has 8 jumps */

    mouve *list[32];
    uint count = 0;
    allValideMouvesKnight(b, true, 4, 4, list, &count);
    EXPECT_EQ(8, count, "allValideMouvesKnight: center knight on open board has 8 moves");
    freeMoveList(list, count);

    FreeBoard(b);
}

void test_allValideMouvesBishop(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[7][4] = W_KING;
    b->grid[0][4] = B_KING;
    b->grid[4][4] = W_BISHOP; /* Center bishop has 13 diagonal squares */

    mouve *list[32];
    uint count = 0;
    allValideMouvesBishop(b, true, 4, 4, list, &count);
    EXPECT_EQ(13, count, "allValideMouvesBishop: center bishop on open board has 13 moves");
    freeMoveList(list, count);

    FreeBoard(b);
}

void test_allValideMouvesRook(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[7][0] = W_KING;
    b->grid[0][0] = B_KING;
    b->grid[4][4] = W_ROOK; /* Center rook unblocked has 14 moves */

    mouve *list[32];
    uint count = 0;
    allValideMouvesRook(b, true, 4, 4, list, &count);
    EXPECT_EQ(14, count, "allValideMouvesRook: center rook on open board has 14 moves");
    freeMoveList(list, count);

    FreeBoard(b);
}

void test_allValideMouvesQueen(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[7][0] = W_KING;
    b->grid[0][0] = B_KING;
    b->grid[4][4] = W_QUEEN; /* 14 rook + 13 bishop = 27 moves */

    mouve *list[64];
    uint count = 0;
    allValideMouvesQueen(b, true, 4, 4, list, &count);
    EXPECT_EQ(27, count, "allValideMouvesQueen: center queen on open board has 27 moves");
    freeMoveList(list, count);

    FreeBoard(b);
}

void test_allValideMouvesKing(void) {
    GameState *b = CreateEmptyTestBoard();
    b->grid[4][4] = W_KING;
    b->grid[0][0] = B_KING;

    mouve *list[32];
    uint count = 0;
    allValideMouvesKing(b, true, 4, 4, list, &count);
    EXPECT_EQ(8, count, "allValideMouvesKing: center king on open board has 8 moves");
    freeMoveList(list, count);

    FreeBoard(b);
}

void test_allValideMouves(void) {
    GameState *b = CreateNullBoard();
    InitiliseBoard(b);

    uint count = 0;
    mouve **moves = allValideMouves(b, true, &count);
    /* 16 pawn moves + 4 knight moves = 20 opening moves */
    EXPECT_EQ(20, count, "allValideMouves: initial chess position has 20 legal moves for White");

    freeMoveList(moves, count);
    free(moves);
    FreeBoard(b);
}

void test_isFiniched(void) {
    GameState *b = CreateNullBoard();
    InitiliseBoard(b);
    EXPECT_EQ(false, isFiniched(b, true), "isFiniched: starting board is not finished");
    FreeBoard(b);

    /* Fool's Mate position against White King at h1 */
    GameState *mate = CreateEmptyTestBoard();
    mate->grid[7][7] = W_KING; /* h1 */
    mate->grid[0][0] = B_KING; /* a8 */
    mate->grid[6][7] = B_QUEEN; /* h2 protected by B_ROOK at h8 */
    mate->grid[0][7] = B_ROOK;  /* h8 */
    mate->grid[5][5] = B_ROOK;  /* f3 covering g1/g2 */
    EXPECT_EQ(true, isFiniched(mate, true), "isFiniched: checkmated White side returns true");
    FreeBoard(mate);
}

/* =========================================================================
 * MOUVESSTOCKFISH.C FUNCTION TESTS
 * ========================================================================= */

void test_Stockfishmouve(void) {
    GameState *b = CreateEmptyTestBoard();
    char response[] = "bestmove e7e5 ponder e2e4";

    mouve *m = Stockfishmouve(b, response);
    EXPECT_EQ(4, m->xStart, "Stockfishmouve: 'e7e5' xStart == 4");
    EXPECT_EQ(1, m->yStart, "Stockfishmouve: 'e7e5' yStart == 1 (8 - 7)");
    EXPECT_EQ(4, m->xEnd, "Stockfishmouve: 'e7e5' xEnd == 4");
    EXPECT_EQ(3, m->yEnd, "Stockfishmouve: 'e7e5' yEnd == 3 (8 - 5)");
    EXPECT_EQ(false, m->isWhite, "Stockfishmouve: sets isWhite to false");

    free(m);
    FreeBoard(b);
}

/* =========================================================================
 * GROUPED SUITES (Backward-compatible with existing CTest names)
 * ========================================================================= */

void suite_utilities(void) {
    test_CreateNullBoard();
    test_FreeBoard();
    test_combineStr();
    test_InitiliseBoard();
    test_PieceToChar();
    test_FromChartoUint();
    test_FromUintToChar();
    test_saveMouve();
    test_Mouve();
    test_getRank();
    test_getRankPiece();
    test_isEmpty();
    test_isEmptyPiece();
    test_isWhite();
    test_isWhitePiece();
    test_cordAllPieces();
    test_getKings();
    test_Stockfishmouve();
}

void suite_pawns(void) {
    test_CheckWhitePawn();
    test_CheckBlackPawn();
    test_isEnPassant();
    test_allValideMouvesPawn();
}

void suite_knights(void) {
    test_CheckWhiteKnight();
    test_CheckBlackKnight();
    test_allValideMouvesKnight();
}

void suite_bishops(void) {
    test_CheckWhiteBishop();
    test_CheckBlackBishop();
    test_allValideMouvesBishop();
}

void suite_rooks(void) {
    test_CheckWhiteRook();
    test_CheckBlackRook();
    test_allValideMouvesRook();
}

void suite_queens(void) {
    test_CheckWhiteQueen();
    test_CheckBlackQueen();
    test_allValideMouvesQueen();
}

void suite_kings(void) {
    test_CheckWhiteKing();
    test_CheckBlackKing();
    test_isKingAttaked();
    test_iscastling();
    test_isMoveLegal();
    test_CheckMouve();
    test_allValideMouvesKing();
    test_allValideMouves();
    test_isFiniched();
}

typedef struct {
    const char *name;
    void (*func)(void);
} TestEntry;

int main(int argc, char *argv[]) {
    TestEntry dispatch[] = {
        /* Legacy suite names */
        {"utilities", suite_utilities},
        {"pawns", suite_pawns},
        {"knights", suite_knights},
        {"bishops", suite_bishops},
        {"rooks", suite_rooks},
        {"queens", suite_queens},
        {"kings", suite_kings},

        /* Per-function tests (bord.c) */
        {"CreateNullBoard", test_CreateNullBoard},
        {"FreeBoard", test_FreeBoard},
        {"combineStr", test_combineStr},
        {"InitiliseBoard", test_InitiliseBoard},
        {"PieceToChar", test_PieceToChar},
        {"FromChartoUint", test_FromChartoUint},
        {"FromUintToChar", test_FromUintToChar},
        {"saveMouve", test_saveMouve},
        {"Mouve", test_Mouve},

        /* Per-function tests (check.c) */
        {"getRank", test_getRank},
        {"getRankPiece", test_getRankPiece},
        {"isEmpty", test_isEmpty},
        {"isEmptyPiece", test_isEmptyPiece},
        {"isWhite", test_isWhite},
        {"isWhitePiece", test_isWhitePiece},
        {"cordAllPieces", test_cordAllPieces},
        {"getKings", test_getKings},
        {"isKingAttaked", test_isKingAttaked},
        {"isMoveLegal", test_isMoveLegal},
        {"isEnPassant", test_isEnPassant},
        {"iscastling", test_iscastling},
        {"CheckWhitePawn", test_CheckWhitePawn},
        {"CheckBlackPawn", test_CheckBlackPawn},
        {"CheckWhiteRook", test_CheckWhiteRook},
        {"CheckBlackRook", test_CheckBlackRook},
        {"CheckWhiteBishop", test_CheckWhiteBishop},
        {"CheckBlackBishop", test_CheckBlackBishop},
        {"CheckWhiteKnight", test_CheckWhiteKnight},
        {"CheckBlackKnight", test_CheckBlackKnight},
        {"CheckWhiteQueen", test_CheckWhiteQueen},
        {"CheckBlackQueen", test_CheckBlackQueen},
        {"CheckWhiteKing", test_CheckWhiteKing},
        {"CheckBlackKing", test_CheckBlackKing},
        {"CheckMouve", test_CheckMouve},
        {"allValideMouvesPawn", test_allValideMouvesPawn},
        {"allValideMouvesKnight", test_allValideMouvesKnight},
        {"allValideMouvesBishop", test_allValideMouvesBishop},
        {"allValideMouvesRook", test_allValideMouvesRook},
        {"allValideMouvesQueen", test_allValideMouvesQueen},
        {"allValideMouvesKing", test_allValideMouvesKing},
        {"allValideMouves", test_allValideMouves},
        {"isFiniched", test_isFiniched},

        /* Per-function tests (mouvesStockfish.c) */
        {"Stockfishmouve", test_Stockfishmouve}
    };

    size_t num_entries = sizeof(dispatch) / sizeof(dispatch[0]);

    if (argc < 2 || strcmp(argv[1], "all") == 0) {
        /* Run every individual function test */
        for (size_t i = 7; i < num_entries; i++) {
            dispatch[i].func();
        }
    } else {
        bool found = false;
        for (size_t i = 0; i < num_entries; i++) {
            if (strcmp(argv[1], dispatch[i].name) == 0) {
                dispatch[i].func();
                found = true;
                break;
            }
        }
        if (!found) {
            printf("Unknown test or suite: %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    if (tests_failed > 0) {
        printf("%d/%d assertions failed.\n", tests_failed, tests_run);
        return EXIT_FAILURE;
    }

    printf("All %d assertions passed!\n", tests_run);
    return EXIT_SUCCESS;
}