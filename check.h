#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include "bord.h"

// Piece-specific and color-specific move validation functions
bool CheckWhitePawn(Board board, mouve *m);
bool CheckBlackPawn(Board board, mouve *m);

bool CheckWhiteKnight(Board board, mouve *m);
bool CheckBlackKnight(Board board, mouve *m);

bool CheckWhiteBishop(Board board, mouve *m);
bool CheckBlackBishop(Board board, mouve *m);

bool CheckWhiteRook(Board board, mouve *m);
bool CheckBlackRook(Board board, mouve *m);

bool CheckWhiteQueen(Board board, mouve *m);
bool CheckBlackQueen(Board board, mouve *m);

bool CheckWhiteKing(Board board, mouve *m);
bool CheckBlackKing(Board board, mouve *m);

// Main checking function
bool CheckMouve(Board board, mouve *m);

#endif // CHECK_H