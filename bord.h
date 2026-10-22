#ifndef BORD_H
#define BORD_H

#include <stdbool.h>
enum pieces
{
    EMPTY = 0,
    B_PAWN = 11,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,
    W_PAWN = 21,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,
};
typedef unsigned int uint;

// Represent the board directly as a 2D dynamically allocated array
typedef uint **Board;

typedef struct
{
    char xStart;
    uint yStart;
    char xEnd;
    uint yEnd;
} mouve;

// Board allocation and lifecycle
uint **CreateNullBoard(void);
void FreeBoard(uint **board);
uint **InitiliseBoard(uint **board);
char PieceToChar(uint piece);
void PrintBoard(uint **board);

// Move operations
mouve *getMouve(void);
void printmouve(uint **board, mouve *m);
uint **Mouve(uint **board, uint piece, uint x, uint y);
bool CheckMouve(uint **board, uint piece, uint x, uint y);

#endif // BORD_H