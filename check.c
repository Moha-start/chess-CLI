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
    if (m->xEnd==m->xStart && m->yEnd==m->yStart) return false;
    bool emptyEnd=isEmpty(board, m->xEnd, m->yEnd);

    if (m->xEnd == m->xStart){
        if (m->yEnd == (m->yStart - 1) && emptyEnd) return true; 
        if (m->yStart == 6 && m->yEnd == (m->yStart - 2) && emptyEnd && isEmpty(board, m->xEnd, m->yStart - 1)) return true;
    } else if (m->xEnd == m->xStart + 1 || m->xEnd == m->xStart - 1){
        if (m->yStart==m->yEnd+1 && !emptyEnd&&!isWhite(board,m->xEnd,m->yEnd))return true;
    }

    return false;
}

bool CheckBlackPawn(Board board, mouve *m) {
    bool emptyEnd=isEmpty(board, m->xEnd, m->yEnd);
    if (m->xEnd==m->xStart && m->yEnd==m->yStart) return false;

    if (m->xEnd == m->xStart){
        if (m->yEnd == (m->yStart + 1) && emptyEnd) return true; 
        if (m->yStart == 1 && m->yEnd == (m->yStart + 2) && emptyEnd && isEmpty(board, m->xEnd, m->yStart + 1)) return true;
    } else if (m->xEnd == m->xStart + 1 || m->xEnd == m->xStart - 1){
        if (m->yStart==m->yEnd-1 && !emptyEnd&&isWhite(board,m->xEnd,m->yEnd))return true;
    }

    return false;
}

bool CheckWhiteRook(Board board, mouve *m) {
    if (m->xEnd != m->xStart && m->yEnd != m->yStart) return false;
    if (m->xEnd == m->xStart && m->yEnd == m->yStart) return false;
    
    if (m->xEnd == m->xStart) {
        if (m->yStart > m->yEnd) {
            for (int yCurent = m->yStart - 1; yCurent > m->yEnd; yCurent--) {
                if (!isEmpty(board, m->xStart, yCurent)) return false;
            }
        } else {
            for (int yCurent = m->yStart + 1; yCurent < m->yEnd; yCurent++) {
                if (!isEmpty(board, m->xStart, yCurent)) return false;
            }
        }
        if (isEmpty(board, m->xEnd, m->yEnd) || !isWhite(board, m->xEnd, m->yEnd)) return true;
    } else {
        if (m->xStart > m->xEnd) {
            for (int xCurent = m->xStart - 1; xCurent > m->xEnd; xCurent--) {
                if (!isEmpty(board, xCurent, m->yStart)) return false;
            }
        } else {
            for (int xCurent = m->xStart + 1; xCurent < m->xEnd; xCurent++) {
                if (!isEmpty(board, xCurent, m->yStart)) return false;
            }
        }
        if (isEmpty(board, m->xEnd, m->yEnd) || !isWhite(board, m->xEnd, m->yEnd)) return true;
    }
    return false;
}
bool CheckBlackRook(Board board, mouve *m) {
    if (m->xEnd != m->xStart && m->yEnd != m->yStart) return false;
    if (m->xEnd == m->xStart && m->yEnd == m->yStart) return false;
    
    if (m->xEnd == m->xStart) {
        if (m->yStart > m->yEnd) {
            for (int yCurent = m->yStart - 1; yCurent > m->yEnd; yCurent--) {
                if (!isEmpty(board, m->xStart, yCurent)) return false;
            }
        } else {
            for (int yCurent = m->yStart + 1; yCurent < m->yEnd; yCurent++) {
                if (!isEmpty(board, m->xStart, yCurent)) return false;
            }
        }
        if (isEmpty(board, m->xEnd, m->yEnd) || isWhite(board, m->xEnd, m->yEnd)) return true;
    } else {
        if (m->xStart > m->xEnd) {
            for (int xCurent = m->xStart - 1; xCurent > m->xEnd; xCurent--) {
                if (!isEmpty(board, xCurent, m->yStart)) return false;
            }
        } else {
            for (int xCurent = m->xStart + 1; xCurent < m->xEnd; xCurent++) {
                if (!isEmpty(board, xCurent, m->yStart)) return false;
            }
        }
        if (isEmpty(board, m->xEnd, m->yEnd) || isWhite(board, m->xEnd, m->yEnd)) return true;
    }
    return false;
}



bool CheckWhiteKnight(Board board, mouve *m) { return false; }
bool CheckBlackKnight(Board board, mouve *m) { return false; }

bool CheckWhiteBishop(Board board, mouve *m) { return false; }
bool CheckBlackBishop(Board board, mouve *m) { return false; }


bool CheckWhiteQueen(Board board, mouve *m) { return false; }
bool CheckBlackQueen(Board board, mouve *m) { return false; }

bool CheckWhiteKing(Board board, mouve *m) { return false; }
bool CheckBlackKing(Board board, mouve *m) { return false; }

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