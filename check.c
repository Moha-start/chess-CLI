#include <stdbool.h>
#include "check.h"

bool CheckWhitePawn(Board board, mouve *m) { return true; }
bool CheckBlackPawn(Board board, mouve *m) { return true; }

bool CheckWhiteKnight(Board board, mouve *m) { return true; }
bool CheckBlackKnight(Board board, mouve *m) { return true; }

bool CheckWhiteBishop(Board board, mouve *m) { return true; }
bool CheckBlackBishop(Board board, mouve *m) { return true; }

bool CheckWhiteRook(Board board, mouve *m) { return true; }
bool CheckBlackRook(Board board, mouve *m) { return true; }

bool CheckWhiteQueen(Board board, mouve *m) { return true; }
bool CheckBlackQueen(Board board, mouve *m) { return true; }

bool CheckWhiteKing(Board board, mouve *m) { return true; }
bool CheckBlackKing(Board board, mouve *m) { return true; }

bool CheckMouve(Board board, mouve *m){
    // Note: ensure your array indexing matches your rank/file logic (board[row][col]).
    uint startpiece = board[m->xStart][m->yStart]; 

    if (startpiece == 0 || (startpiece / 10 != 2 && startpiece / 10 != 1)) return false;
    if (startpiece / 10 != (m->isWhite ? 1 : 2)) return false;

    switch (startpiece){
    case B_PAWN:   return CheckBlackPawn(board, m);
    case W_PAWN:   return CheckWhitePawn(board, m);
    case B_KNIGHT: return CheckBlackKnight(board, m);
    case W_KNIGHT: return CheckWhiteKnight(board, m);
    case B_BISHOP: return CheckBlackBishop(board, m);
    case W_BISHOP: return CheckWhiteBishop(board, m);
    case B_ROOK:   return CheckBlackRook(board, m);
    case W_ROOK:   return CheckWhiteRook(board, m);
    case B_QUEEN:  return CheckBlackQueen(board, m);
    case W_QUEEN:  return CheckWhiteQueen(board, m);
    case B_KING:   return CheckBlackKing(board, m);
    case W_KING:   return CheckWhiteKing(board, m);
    default:
        return false;
    }
}