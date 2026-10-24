#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "check.h"

bool isEmpty(Board board, uint x, uint y) {
    return board[y][x] == EMPTY;
}

bool isWhite(Board board, uint x, uint y) {
    uint piece = board[y][x];
    if (piece == 0) {
        fprintf(stderr, "there is no color for an empty place");
        exit(EXIT_FAILURE);
    }
    return piece / 10 == 1 ? true : false;
}

bool CheckWhitePawn(Board board, mouve *m) {
    if (m->yStart == 6) {
        if (m->yEnd == (m->yStart - 1) && isEmpty(board, m->xEnd, m->yEnd)) {
            return true;
        } else if (m->yEnd == (m->yStart - 2) && isEmpty(board, m->xEnd, m->yEnd)) {
            return true;
        }
    } else {
        if (m->yEnd == (m->yStart - 1)) {
            if (isEmpty(board, m->xEnd, m->yEnd)) {
                return true;
            }
        }
    }    
    return false;
}

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

bool CheckMouve(Board board, mouve *m) {
    uint startpiece = board[m->yStart][m->xStart]; 

    if (startpiece == 0 || (startpiece / 10 != 2 && startpiece / 10 != 1)) return false;
    if (startpiece / 10 != (m->isWhite ? 1 : 2)) return false;

    switch (startpiece) {
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