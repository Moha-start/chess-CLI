#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "check.h"

uint getRank(GameState *board, uint x, uint y){
    return board->grid[y][x]%10;
}
uint getRankPiece(uint piece){
    return piece%10;
}

bool isEnPassant(GameState *board, mouve *m) {
    if (isEmpty(board, m->xStart, m->yStart)) return false;
    
    uint startpiece = board->grid[m->yStart][m->xStart];
    
    // 1. Is it actually a pawn attempting the move?
    if (getRankPiece(startpiece) != PAWN) return false;
    if (isWhitePiece(startpiece) != m->isWhite) return false;

    // 2. Is the attempt a diagonal move into an empty square?
    if (!isEmpty(board, m->xEnd, m->yEnd)) return false;

    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    if (difx != 1) return false;

    if (m->isWhite) {
        if (m->yEnd != m->yStart - 1) return false;
    } else {
        if (m->yEnd != m->yStart + 1) return false;
    }

    // 3. Verify the opponent's previous move allows en passant
    if (!board->hasLastMove) return false;

    mouve *last = &board->lastMovePlayed;
    uint lastPiece = board->grid[last->yEnd][last->xEnd];

    // Did the opponent move a pawn?
    if (getRankPiece(lastPiece) != PAWN) return false;
    if (isWhitePiece(lastPiece) == m->isWhite) return false;

    // Did that pawn move exactly two squares?
    uint lastDifY = last->yStart > last->yEnd ? last->yStart - last->yEnd : last->yEnd - last->yStart;
    if (lastDifY != 2) return false;

    // Did that pawn land directly beside our pawn?
    // (It must land on the same row we start from, in the column we are moving into)
    if (last->xEnd != m->xEnd || last->yEnd != m->yStart) return false;

    return true;
}

bool isEmpty(GameState *board, uint x, uint y) {
    return board->grid[y][x] == EMPTY;
}

bool isWhite(GameState *board, uint x, uint y) {
    uint piece = board->grid[y][x];
    if (piece == 0) {
        fprintf(stderr, "there is no color for an empty place");
        exit(EXIT_FAILURE);
    }
    return piece / 10 == 1 ? true : false;
}

bool isWhitePiece(uint piece){
    if (piece == 0) {
        fprintf(stderr, "there is no color for an empty place");
        exit(EXIT_FAILURE);
    }
    return piece / 10 == 1 ? true : false;
}

bool isKingAttaked(GameState *board, uint x, uint y, bool isWhiteKing){
    int list[3] = {-1, 0, 1};
    for (uint i = 0; i < 3; i++) {
        for (uint j = 0; j < 3; j++) {
            if (i == 1 && j == 1) continue; 
            
            int xNew = (int)x + list[i];
            int yNew = (int)y + list[j];
            
            if (xNew >= 0 && xNew <= 7 && yNew >= 0 && yNew <= 7) {
                if (!isEmpty(board, xNew, yNew) && isWhiteKing != isWhite(board, xNew, yNew)) {
                    int pieceType = getRank(board, xNew, yNew);
                    if (pieceType == KING) return true;
                    if (pieceType ==PAWN  && list[i] != 0 && list[j] != 0) {
                        int pawnAttackDir = isWhiteKing ? 1 : -1; 
                        if (list[j] == pawnAttackDir) return true;
                    }
                }
            }
        }
    }
    
    const int moves[8][2] = {
        { 1,  2}, { 1, -2}, {-1,  2}, {-1, -2},
        { 2,  1}, { 2, -1}, {-2,  1}, {-2, -1}
    };

    for (int i = 0; i < 8; i++) {
        int xNew = (int)x + moves[i][0];
        int yNew = (int)y + moves[i][1];
        if (xNew >= 0 && xNew <= 7 && yNew >= 0 && yNew <= 7) {
            if (!isEmpty(board, xNew, yNew) && isWhiteKing != isWhite(board, xNew, yNew)) {
                if (getRank(board, xNew, yNew) == KNIGHT) return true;
            }
        }
    }
    
    int xUp = (int)x + 1;
    for (; xUp < 8 && isEmpty(board, xUp, y); xUp++);
    if (xUp < 8) {
        if (isWhiteKing != isWhite(board, xUp, y) && (getRank(board, xUp, y) == ROOK || getRank(board, xUp, y) == QUEEN)) return true;
    }
    
    int xDown = (int)x - 1;
    for (; xDown >= 0 && isEmpty(board, xDown, y); xDown--);
    if (xDown >= 0) {
        if (isWhiteKing != isWhite(board, xDown, y) && (getRank(board, xDown, y) == ROOK || getRank(board, xDown, y) == QUEEN)) return true;
    }
    
    int yLeft = (int)y - 1;
    for (; yLeft >= 0 && isEmpty(board, x, yLeft); yLeft--);
    if (yLeft >= 0) {
        if (isWhiteKing != isWhite(board, x, yLeft) && (getRank(board, x, yLeft) == ROOK || getRank(board, x, yLeft) == QUEEN)) return true;
    }
    
    int yRight = (int)y + 1;
    for (; yRight < 8 && isEmpty(board, x, yRight); yRight++);
    if (yRight < 8) {
        if (isWhiteKing != isWhite(board, x, yRight) && (getRank(board, x, yRight) == ROOK || getRank(board, x, yRight) == QUEEN)) return true;
    }

    int xNewtr, yNewtr;
    for (xNewtr = x + 1, yNewtr = y + 1; (xNewtr < 8 && yNewtr < 8) && isEmpty(board, xNewtr, yNewtr); xNewtr++, yNewtr++);
    if ((xNewtr < 8 && yNewtr < 8) && isWhiteKing != isWhite(board, xNewtr, yNewtr) && (getRank(board, xNewtr, yNewtr) == BISHOP || getRank(board, xNewtr, yNewtr) == QUEEN)) return true;

    int xNewtl, yNewtl;
    for (xNewtl = x + 1, yNewtl = y - 1; (xNewtl < 8 && yNewtl > -1) && isEmpty(board, xNewtl, yNewtl); xNewtl++, yNewtl--);
    if ((xNewtl < 8 && yNewtl > -1) && isWhiteKing != isWhite(board, xNewtl, yNewtl) && (getRank(board, xNewtl, yNewtl) == BISHOP || getRank(board, xNewtl, yNewtl) == QUEEN)) return true;

    int xNewdr, yNewdr;
    for (xNewdr = x - 1, yNewdr = y + 1; (xNewdr > -1 && yNewdr < 8) && isEmpty(board, xNewdr, yNewdr); xNewdr--, yNewdr++);
    if ((xNewdr > -1 && yNewdr < 8) && isWhiteKing != isWhite(board, xNewdr, yNewdr) && (getRank(board, xNewdr, yNewdr) == BISHOP || getRank(board, xNewdr, yNewdr) == QUEEN)) return true;

    int xNewdl, yNewdl;
    for (xNewdl = x - 1, yNewdl = y - 1; (xNewdl > -1 && yNewdl > -1) && isEmpty(board, xNewdl, yNewdl); xNewdl--, yNewdl--);
    if ((xNewdl > -1 && yNewdl > -1) && isWhiteKing != isWhite(board, xNewdl, yNewdl) && (getRank(board, xNewdl, yNewdl) == BISHOP || getRank(board, xNewdl, yNewdl) == QUEEN)) return true;
    
    return false;
}

bool iscastling(GameState *board,mouve *m){
    uint dest=board->grid[m->yEnd][m->xEnd];
    if(dest == EMPTY || getRankPiece(dest)!=ROOK)return false;
    uint start=board->grid[m->yStart][m->xStart];
    if(isWhitePiece(start)!=isWhitePiece(dest))return false;
    uint color = isWhitePiece(start) ? 0 : 1;
    uint pos = m->xEnd / 7;
    if (board->KingMouved[color]&&board->rookmouved[color][pos])return true;
    return false;
}

bool CheckWhitePawn(GameState *board, mouve *m) {
    if (m->xEnd==m->xStart && m->yEnd==m->yStart) return false;
    bool emptyEnd = isEmpty(board, m->xEnd, m->yEnd);

    if (m->xEnd == m->xStart){
        if (m->yEnd == (m->yStart - 1) && emptyEnd) return true; 
        if (m->yStart == 6 && m->yEnd == (m->yStart - 2) && emptyEnd && isEmpty(board, m->xEnd, m->yStart - 1)) return true;
    } else if (m->xEnd == m->xStart + 1 || m->xEnd == m->xStart - 1){
        if (m->yStart == m->yEnd + 1) {
            if (!emptyEnd && !isWhite(board, m->xEnd, m->yEnd)) return true; // Capture normale
            if (isEnPassant(board, m)) return true; // Capture en passant
        }
    }

    return false;
}

bool CheckBlackPawn(GameState *board, mouve *m) {
    bool emptyEnd = isEmpty(board, m->xEnd, m->yEnd);
    if (m->xEnd==m->xStart && m->yEnd==m->yStart) return false;

    if (m->xEnd == m->xStart){
        if (m->yEnd == (m->yStart + 1) && emptyEnd) return true; 
        if (m->yStart == 1 && m->yEnd == (m->yStart + 2) && emptyEnd && isEmpty(board, m->xEnd, m->yStart + 1)) return true;
    } else if (m->xEnd == m->xStart + 1 || m->xEnd == m->xStart - 1){
        if (m->yStart == m->yEnd - 1) {
            if (!emptyEnd && isWhite(board, m->xEnd, m->yEnd)) return true; // Capture normale
            if (isEnPassant(board, m)) return true; // Capture en passant
        }
    }

    return false;
}

bool CheckWhiteRook(GameState *board, mouve *m) {
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

bool CheckBlackRook(GameState *board, mouve *m) {
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

bool CheckWhiteBishop(GameState *board, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    
    if (dify != difx) return false;
    
    int stepX = (m->xEnd > m->xStart) ? 1 : -1;
    int stepY = (m->yEnd > m->yStart) ? 1 : -1;
    
    uint x = m->xStart + stepX;
    uint y = m->yStart + stepY;
    
    while (x != m->xEnd) {
        if (!isEmpty(board, x, y)) return false;
        x += stepX;
        y += stepY;
    }

    if (isEmpty(board, m->xEnd, m->yEnd) || !isWhite(board, m->xEnd, m->yEnd)) return true;

    return false; 
}

bool CheckBlackBishop(GameState *board, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    
    if (dify != difx) return false;
    
    int stepX = (m->xEnd > m->xStart) ? 1 : -1;
    int stepY = (m->yEnd > m->yStart) ? 1 : -1;
    
    uint x = m->xStart + stepX;
    uint y = m->yStart + stepY;
    
    while (x != m->xEnd) {
        if (!isEmpty(board, x, y)) return false;
        x += stepX;
        y += stepY;
    }

    if (isEmpty(board, m->xEnd, m->yEnd) || isWhite(board, m->xEnd, m->yEnd)) return true;

    return false; 
}

bool CheckWhiteKnight(GameState *board, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if (difx+dify!=3) return false;
    if (isEmpty(board,m->xEnd,m->yEnd)||!isWhite(board,m->xEnd,m->yEnd)) return true;
    return false; 
}

bool CheckBlackKnight(GameState *board, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if (difx+dify!=3) return false;
    if (isEmpty(board,m->xEnd,m->yEnd)||isWhite(board,m->xEnd,m->yEnd)) return true;
    return false; 
}

bool CheckWhiteQueen(GameState *board, mouve *m) {
    return CheckWhiteRook(board,m)||CheckWhiteBishop(board,m); 
}

bool CheckBlackQueen(GameState *board, mouve *m) {
    return CheckBlackRook(board,m)||CheckBlackBishop(board,m); 
}

bool CheckWhiteKing(GameState *board, mouve *m) {
    if (isKingAttaked(board, m->xEnd, m->yEnd, true)) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if ((difx <= 1 && dify <= 1) && (difx != 0 || dify != 0)){
        if (isEmpty(board,m->xEnd,m->yEnd)||!isWhite(board,m->xEnd,m->yEnd)) return true;
    }
    return false; 
}

bool CheckBlackKing(GameState *board, mouve *m) {
    if (isKingAttaked(board,m->xEnd,m->yEnd,isWhite(board,m->xStart,m->yStart))) return false;
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if ((difx <= 1 && dify <= 1) && (difx != 0 || dify != 0)){
        if (isEmpty(board,m->xEnd,m->yEnd) || isWhite(board,m->xEnd,m->yEnd)) return true;
    }
    return false; 
}

bool CheckMouve(GameState *board, mouve *m) {
    if (isEmpty(board, m->xStart, m->yStart)) return false;
    uint startpiece = board->grid[m->yStart][m->xStart];

    if (isWhite(board, m->xStart, m->yStart) != m->isWhite) return false;

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