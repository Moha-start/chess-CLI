#ifndef BORD_H
#define BORD_H

#include <stdbool.h>

enum pieces
{
    EMPTY = 0,
    B_PAWN = 21,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,
    W_PAWN = 11,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,
};
typedef unsigned int uint;

typedef struct {
    uint **grid;
    uint emptyPieces[2][13];
} GameState;

typedef struct{
    uint xStart;
    uint yStart;
    uint xEnd;
    uint yEnd;
    bool isWhite;
} mouve;

// Board allocation and lifecycle
GameState *CreateNullBoard(void);
void FreeBoard(GameState *state);
void InitiliseBoard(GameState *state);
char PieceToChar(uint piece);
void PrintBoard(GameState *state);
uint FromChartoUint(char c);
char FromUintToChar(uint n);

// Move operations
mouve *getMouve(bool isWhite);
void printmouve(GameState *state, mouve *m);
bool Mouve(GameState *state, mouve *m);

#endif // BORD_H