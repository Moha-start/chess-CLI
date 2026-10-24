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

// Represent the board directly as a 2D dynamically allocated array
typedef uint **Board;

typedef struct{
    uint xStart;
    uint yStart;
    uint xEnd;
    uint yEnd;
    bool isWhite;
} mouve;

// Board allocation and lifecycle
Board CreateNullBoard(void);
void FreeBoard(Board board);
void InitiliseBoard(Board board);
char PieceToChar(uint piece);
void PrintBoard(Board board);
uint FromChartoUint(char c);
char FromUintToChar(uint n);

// Move operations
mouve *getMouve(bool isWhite);
void printmouve(Board board, mouve *m);
bool Mouve(Board board, mouve *m);

#endif // BORD_H