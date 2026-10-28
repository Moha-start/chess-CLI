#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "check.h"

uint getRank(GameState *state, uint x, uint y){
    return state->grid[y][x]%10;
}

uint getRankPiece(uint piece){
    return piece%10;
}

uint ** cordAllPieces(GameState *state, bool isPWhite) {
    uint **plays = malloc(16 * sizeof(uint *));
    if (plays == NULL) {
        FreeBoard(state);
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    uint index = 0;
    for (uint i = 0; i < 16; i++) {
        plays[i] = malloc(2 * sizeof(uint));
        if (plays[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            for (uint j = 0; j < i; j++) {
                free(plays[j]);
            }
            free(plays);
            FreeBoard(state);
            exit(EXIT_FAILURE);
        }
    }
    
    for (uint i = 0; i < BOARDSIZE; i++) {
        for (uint j = 0; j < BOARDSIZE; j++) {
            uint piece = state->grid[i][j];
            if (!isEmptyPiece(piece) && isWhitePiece(piece) == isPWhite) {
                plays[index][0] = i;
                plays[index][1] = j;
                index++; 
            }
        }
    }
    return plays;
}
void allValideMouvesPawn(GameState *state,bool isPWhite,uint x,uint y,mouve ** valide,uint *indexmv){
    int newcords[][2] = {
        {x+1, y}, 
        {x-1, y}, 
        {x+1, y-1}, 
        {x+1, y+1},
        {x-1, y-1}, 
        {x-1, y+1}
    };
    for (uint i=0;i<6;i++){
        if((newcords[i][0]>-1&&newcords[i][0]<8)&&(newcords[i][1]>-1&&newcords[i][1]<8)){
            mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=newcords[i][0];
            mv->yEnd=newcords[i][1];
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
    }
}
void allValideMouvesKnight(GameState *state,bool isPWhite,uint x,uint y,mouve ** valide,uint *indexmv){
    int newcords[][2] = {
        {x+2, y+1}, 
        {x+2, y-1}, 
        {x-2, y+1}, 
        {x-2, y-1},
        {x+1, y+2}, 
        {x+1, y-2}, 
        {x-1, y+2}, 
        {x-1, y-2}
    };
    for (uint i=0;i<6;i++){
        if((newcords[i][0]>-1&&newcords[i][0]<8)&&(newcords[i][1]>-1&&newcords[i][1]<8)){
            mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=newcords[i][0];
            mv->yEnd=newcords[i][1];
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
    }       

}
void allValideMouvesBishop(GameState *state,bool isPWhite,uint x,uint y,mouve ** valide,uint *indexmv){
    for (int xUpLeft=x++,yUpLeft=y++;xUpLeft<BOARDSIZE&&yUpLeft<BOARDSIZE;xUpLeft++,yUpLeft++){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=xUpLeft;
            mv->yEnd=yUpLeft;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
    for (int xUpRight=x++,yUpRight=y--;xUpRight<BOARDSIZE&&yUpRight>-1;xUpRight++,yUpRight--){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=xUpRight;
            mv->yEnd=yUpRight;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }      
    for (int xDownRight=x--,yDownRight=y--;xDownRight>-1&&yDownRight>-1;xDownRight--,yDownRight--){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=xDownRight;
            mv->yEnd=yDownRight;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
    for (int xDownLeft=x--,yDownLeft=y++;xDownLeft>-1&&yDownLeft<BOARDSIZE;xDownLeft--,yDownLeft++){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=xDownLeft;
            mv->yEnd=yDownLeft;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
}
void allValideMouvesRook(GameState *state,bool isPWhite,uint x,uint y,mouve ** valide,uint *indexmv){
    for (uint xUp=x++;xUp<BOARDSIZE;xUp++){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=xUp;
            mv->yEnd=y;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
    for (int xDown=x--;xDown>-1;xDown--){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=xDown;
            mv->yEnd=y;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
    for (int yRight=y++;yRight<BOARDSIZE;yRight++){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=x;
            mv->yEnd=yRight;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
    for (int yLeft=y--;yLeft>-1;yLeft--){
        mouve *mv = malloc(sizeof(mouve));
            if (mv == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            mv->isWhite=isPWhite;
            mv->xStart=x;
            mv->yStart=y;
            mv->xEnd=x;
            mv->yEnd=yLeft;
            if (CheckMouve(state,mv)){
                valide[*indexmv]=mv;
                (*indexmv)++;
            }else {
                free(mv);
            }
        }
            
}
void allValideMouvesQueen(GameState *state,bool isPWhite,uint x,uint y,mouve ** valide,uint *indexmv){
    allValideMouvesRook(state,isPWhite,x,y,valide,indexmv);
    allValideMouvesBishop(state,isPWhite,x,y,valide,indexmv);
}
void allValideMouvesKing(GameState *state,bool isPWhite,uint x,uint y,mouve ** valide,uint *indexmv);
mouve ** allValideMouves(GameState *state,bool isPWhite){
    mouve ** valide=malloc(150 * sizeof(mouve *));
    uint indexmv=0; 
    uint ** cords=cordAllPieces(state,isPWhite);
    for (uint index=0;index<16;index++){
        uint y = cords[index][0];
        uint x = cords[index][1];
        uint piece=getRank(state,x,y);
        switch (piece) {
        case PAWN : allValideMouvesPawn(state,isPWhite,x,y,valide,indexmv);
        case BISHOP : allValideMouvesBishop(state,isPWhite,x,y,valide,indexmv);
        case ROOK : allValideMouvesRook(state,isPWhite,x,y,valide,indexmv);
        case QUEEN : allValideMouvesQueen(state,isPWhite,x,y,valide,indexmv);
        case KING : allValideMouvesKing(state,isPWhite,x,y,valide,indexmv);
        }
        
        
    }
}

void getKings(GameState *state,uint * coord[]){
   for (uint i=0;i<BOARDSIZE;i++){
    for(uint j=0;j<BOARDSIZE;j++){
        if(getRank(state,i,j)==KING){
            uint color=isWhite(state,i,j) ? 0 : 1;
            coord[color][0]=i;
            coord[color][1]=j;
        }
    }
   }

}

bool isEnPassant(GameState *state, mouve *m) {
    if (isEmpty(state, m->xStart, m->yStart)) return false;
    
    uint startpiece = state->grid[m->yStart][m->xStart];
    
    // 1. Is it actually a pawn attempting the move?
    if (getRankPiece(startpiece) != PAWN) return false;
    if (isWhitePiece(startpiece) != m->isWhite) return false;

    // 2. Is the attempt a diagonal move into an empty square?
    if (!isEmpty(state, m->xEnd, m->yEnd)) return false;

    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    if (difx != 1) return false;

    if (m->isWhite) {
        if (m->yEnd != m->yStart - 1) return false;
    } else {
        if (m->yEnd != m->yStart + 1) return false;
    }

    // 3. Verify the opponent's previous move allows en passant
    if (!state->hasLastMove) return false;

    mouve *last = &state->lastMovePlayed;
    uint lastPiece = state->grid[last->yEnd][last->xEnd];

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

bool isEmpty(GameState *state, uint x, uint y) {
    return state->grid[y][x] == EMPTY;
}

bool isEmptyPiece(uint piece) {
    return piece == EMPTY;
}

bool isWhite(GameState *state, uint x, uint y) {
    uint piece = state->grid[y][x];
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

bool isKingAttaked(GameState *state, uint x, uint y, bool isWhiteKing){
    int list[3] = {-1, 0, 1};
    for (uint i = 0; i < 3; i++) {
        for (uint j = 0; j < 3; j++) {
            if (i == 1 && j == 1) continue; 
            
            int xNew = (int)x + list[i];
            int yNew = (int)y + list[j];
            
            if (xNew >= 0 && xNew <= 7 && yNew >= 0 && yNew <= 7) {
                if (!isEmpty(state, xNew, yNew) && isWhiteKing != isWhite(state, xNew, yNew)) {
                    int pieceType = getRank(state, xNew, yNew);
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
            if (!isEmpty(state, xNew, yNew) && isWhiteKing != isWhite(state, xNew, yNew)) {
                if (getRank(state, xNew, yNew) == KNIGHT) return true;
            }
        }
    }
    
    int xUp = (int)x + 1;
    for (; xUp < 8 && isEmpty(state, xUp, y); xUp++);
    if (xUp < 8) {
        if (isWhiteKing != isWhite(state, xUp, y) && (getRank(state, xUp, y) == ROOK || getRank(state, xUp, y) == QUEEN)) return true;
    }
    
    int xDown = (int)x - 1;
    for (; xDown >= 0 && isEmpty(state, xDown, y); xDown--);
    if (xDown >= 0) {
        if (isWhiteKing != isWhite(state, xDown, y) && (getRank(state, xDown, y) == ROOK || getRank(state, xDown, y) == QUEEN)) return true;
    }
    
    int yLeft = (int)y - 1;
    for (; yLeft >= 0 && isEmpty(state, x, yLeft); yLeft--);
    if (yLeft >= 0) {
        if (isWhiteKing != isWhite(state, x, yLeft) && (getRank(state, x, yLeft) == ROOK || getRank(state, x, yLeft) == QUEEN)) return true;
    }
    
    int yRight = (int)y + 1;
    for (; yRight < 8 && isEmpty(state, x, yRight); yRight++);
    if (yRight < 8) {
        if (isWhiteKing != isWhite(state, x, yRight) && (getRank(state, x, yRight) == ROOK || getRank(state, x, yRight) == QUEEN)) return true;
    }

    int xNewtr, yNewtr;
    for (xNewtr = x + 1, yNewtr = y + 1; (xNewtr < 8 && yNewtr < 8) && isEmpty(state, xNewtr, yNewtr); xNewtr++, yNewtr++);
    if ((xNewtr < 8 && yNewtr < 8) && isWhiteKing != isWhite(state, xNewtr, yNewtr) && (getRank(state, xNewtr, yNewtr) == BISHOP || getRank(state, xNewtr, yNewtr) == QUEEN)) return true;

    int xNewtl, yNewtl;
    for (xNewtl = x + 1, yNewtl = y - 1; (xNewtl < 8 && yNewtl > -1) && isEmpty(state, xNewtl, yNewtl); xNewtl++, yNewtl--);
    if ((xNewtl < 8 && yNewtl > -1) && isWhiteKing != isWhite(state, xNewtl, yNewtl) && (getRank(state, xNewtl, yNewtl) == BISHOP || getRank(state, xNewtl, yNewtl) == QUEEN)) return true;

    int xNewdr, yNewdr;
    for (xNewdr = x - 1, yNewdr = y + 1; (xNewdr > -1 && yNewdr < 8) && isEmpty(state, xNewdr, yNewdr); xNewdr--, yNewdr++);
    if ((xNewdr > -1 && yNewdr < 8) && isWhiteKing != isWhite(state, xNewdr, yNewdr) && (getRank(state, xNewdr, yNewdr) == BISHOP || getRank(state, xNewdr, yNewdr) == QUEEN)) return true;

    int xNewdl, yNewdl;
    for (xNewdl = x - 1, yNewdl = y - 1; (xNewdl > -1 && yNewdl > -1) && isEmpty(state, xNewdl, yNewdl); xNewdl--, yNewdl--);
    if ((xNewdl > -1 && yNewdl > -1) && isWhiteKing != isWhite(state, xNewdl, yNewdl) && (getRank(state, xNewdl, yNewdl) == BISHOP || getRank(state, xNewdl, yNewdl) == QUEEN)) return true;
    
    return false;
}

bool iscastling(GameState *state,mouve *m){
    uint dest=state->grid[m->yEnd][m->xEnd];
    if(dest == EMPTY || getRankPiece(dest)!=ROOK)return false;
    uint start=state->grid[m->yStart][m->xStart];
    if(isWhitePiece(start)!=isWhitePiece(dest))return false;
    uint color = isWhitePiece(start) ? 0 : 1;
    uint pos = m->xEnd / 7;
    if (state->KingMouved[color]&&state->rookmouved[color][pos])return true;
    return false;
}

bool CheckWhitePawn(GameState *state, mouve *m) {
    if (m->xEnd==m->xStart && m->yEnd==m->yStart) return false;
    bool emptyEnd = isEmpty(state, m->xEnd, m->yEnd);

    if (m->xEnd == m->xStart){
        if (m->yEnd == (m->yStart - 1) && emptyEnd) return true; 
        if (m->yStart == 6 && m->yEnd == (m->yStart - 2) && emptyEnd && isEmpty(state, m->xEnd, m->yStart - 1)) return true;
    } else if (m->xEnd == m->xStart + 1 || m->xEnd == m->xStart - 1){
        if (m->yStart == m->yEnd + 1) {
            if (!emptyEnd && !isWhite(state, m->xEnd, m->yEnd)) return true; // Capture normale
            if (isEnPassant(state, m)) return true; // Capture en passant
        }
    }

    return false;
}

bool CheckBlackPawn(GameState *state, mouve *m) {
    bool emptyEnd = isEmpty(state, m->xEnd, m->yEnd);
    if (m->xEnd==m->xStart && m->yEnd==m->yStart) return false;

    if (m->xEnd == m->xStart){
        if (m->yEnd == (m->yStart + 1) && emptyEnd) return true; 
        if (m->yStart == 1 && m->yEnd == (m->yStart + 2) && emptyEnd && isEmpty(state, m->xEnd, m->yStart + 1)) return true;
    } else if (m->xEnd == m->xStart + 1 || m->xEnd == m->xStart - 1){
        if (m->yStart == m->yEnd - 1) {
            if (!emptyEnd && isWhite(state, m->xEnd, m->yEnd)) return true; // Capture normale
            if (isEnPassant(state, m)) return true; // Capture en passant
        }
    }

    return false;
}

bool CheckWhiteRook(GameState *state, mouve *m) {
    if (m->xEnd != m->xStart && m->yEnd != m->yStart) return false;
    if (m->xEnd == m->xStart && m->yEnd == m->yStart) return false;
    
    if (m->xEnd == m->xStart) {
        if (m->yStart > m->yEnd) {
            for (int yCurent = m->yStart - 1; yCurent > m->yEnd; yCurent--) {
                if (!isEmpty(state, m->xStart, yCurent)) return false;
            }
        } else {
            for (int yCurent = m->yStart + 1; yCurent < m->yEnd; yCurent++) {
                if (!isEmpty(state, m->xStart, yCurent)) return false;
            }
        }
        if (isEmpty(state, m->xEnd, m->yEnd) || !isWhite(state, m->xEnd, m->yEnd)) return true;
    } else {
        if (m->xStart > m->xEnd) {
            for (int xCurent = m->xStart - 1; xCurent > m->xEnd; xCurent--) {
                if (!isEmpty(state, xCurent, m->yStart)) return false;
            }
        } else {
            for (int xCurent = m->xStart + 1; xCurent < m->xEnd; xCurent++) {
                if (!isEmpty(state, xCurent, m->yStart)) return false;
            }
        }
        if (isEmpty(state, m->xEnd, m->yEnd) || !isWhite(state, m->xEnd, m->yEnd)) return true;
    }
    return false;
}

bool CheckBlackRook(GameState *state, mouve *m) {
    if (m->xEnd != m->xStart && m->yEnd != m->yStart) return false;
    if (m->xEnd == m->xStart && m->yEnd == m->yStart) return false;
    
    if (m->xEnd == m->xStart) {
        if (m->yStart > m->yEnd) {
            for (int yCurent = m->yStart - 1; yCurent > m->yEnd; yCurent--) {
                if (!isEmpty(state, m->xStart, yCurent)) return false;
            }
        } else {
            for (int yCurent = m->yStart + 1; yCurent < m->yEnd; yCurent++) {
                if (!isEmpty(state, m->xStart, yCurent)) return false;
            }
        }
        if (isEmpty(state, m->xEnd, m->yEnd) || isWhite(state, m->xEnd, m->yEnd)) return true;
    } else {
        if (m->xStart > m->xEnd) {
            for (int xCurent = m->xStart - 1; xCurent > m->xEnd; xCurent--) {
                if (!isEmpty(state, xCurent, m->yStart)) return false;
            }
        } else {
            for (int xCurent = m->xStart + 1; xCurent < m->xEnd; xCurent++) {
                if (!isEmpty(state, xCurent, m->yStart)) return false;
            }
        }
        if (isEmpty(state, m->xEnd, m->yEnd) || isWhite(state, m->xEnd, m->yEnd)) return true;
    }
    return false;
}

bool CheckWhiteBishop(GameState *state, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    
    if (dify != difx) return false;
    
    int stepX = (m->xEnd > m->xStart) ? 1 : -1;
    int stepY = (m->yEnd > m->yStart) ? 1 : -1;
    
    uint x = m->xStart + stepX;
    uint y = m->yStart + stepY;
    
    while (x != m->xEnd) {
        if (!isEmpty(state, x, y)) return false;
        x += stepX;
        y += stepY;
    }

    if (isEmpty(state, m->xEnd, m->yEnd) || !isWhite(state, m->xEnd, m->yEnd)) return true;

    return false; 
}

bool CheckBlackBishop(GameState *state, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    
    if (dify != difx) return false;
    
    int stepX = (m->xEnd > m->xStart) ? 1 : -1;
    int stepY = (m->yEnd > m->yStart) ? 1 : -1;
    
    uint x = m->xStart + stepX;
    uint y = m->yStart + stepY;
    
    while (x != m->xEnd) {
        if (!isEmpty(state, x, y)) return false;
        x += stepX;
        y += stepY;
    }

    if (isEmpty(state, m->xEnd, m->yEnd) || isWhite(state, m->xEnd, m->yEnd)) return true;

    return false; 
}

bool CheckWhiteKnight(GameState *state, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if (difx+dify!=3) return false;
    if (isEmpty(state,m->xEnd,m->yEnd)||!isWhite(state,m->xEnd,m->yEnd)) return true;
    return false; 
}

bool CheckBlackKnight(GameState *state, mouve *m) {
    if (m->xEnd == m->xStart || m->yEnd == m->yStart) return false;
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if (difx+dify!=3) return false;
    if (isEmpty(state,m->xEnd,m->yEnd)||isWhite(state,m->xEnd,m->yEnd)) return true;
    return false; 
}

bool CheckWhiteQueen(GameState *state, mouve *m) {
    return CheckWhiteRook(state,m)||CheckWhiteBishop(state,m); 
}

bool CheckBlackQueen(GameState *state, mouve *m) {
    return CheckBlackRook(state,m)||CheckBlackBishop(state,m); 
}

bool CheckWhiteKing(GameState *state, mouve *m) {
    if (isKingAttaked(state, m->xEnd, m->yEnd, true)) return false;
    
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if ((difx <= 1 && dify <= 1) && (difx != 0 || dify != 0)){
        if (isEmpty(state,m->xEnd,m->yEnd)||!isWhite(state,m->xEnd,m->yEnd)) return true;
    }
    return false; 
}

bool CheckBlackKing(GameState *state, mouve *m) {
    if (isKingAttaked(state,m->xEnd,m->yEnd,isWhite(state,m->xStart,m->yStart))) return false;
    uint difx = m->xStart > m->xEnd ? m->xStart - m->xEnd : m->xEnd - m->xStart;
    uint dify = m->yStart > m->yEnd ? m->yStart - m->yEnd : m->yEnd - m->yStart;
    if ((difx <= 1 && dify <= 1) && (difx != 0 || dify != 0)){
        if (isEmpty(state,m->xEnd,m->yEnd) || isWhite(state,m->xEnd,m->yEnd)) return true;
    }
    return false; 
}
bool isFiniched(GameState *state,bool isWhite){
    uint coord[2][2];
    getKings(state,coord);

}
bool CheckMouve(GameState *state, mouve *m) {
    if (isEmpty(state, m->xStart, m->yStart)) return false;
    uint startpiece = state->grid[m->yStart][m->xStart];

    if (isWhite(state, m->xStart, m->yStart) != m->isWhite) return false;

    switch (startpiece) {
    case B_PAWN:   return CheckBlackPawn(state, m);
    case W_PAWN:   return CheckWhitePawn(state, m);
    case B_KNIGHT: return CheckBlackKnight(state, m);
    case W_KNIGHT: return CheckWhiteKnight(state, m);
    case B_BISHOP: return CheckBlackBishop(state, m);
    case W_BISHOP: return CheckWhiteBishop(state, m);
    case B_ROOK:   return CheckBlackRook(state, m);
    case W_ROOK:   return CheckWhiteRook(state, m);
    case B_QUEEN:  return CheckBlackQueen(state, m);
    case W_QUEEN:  return CheckWhiteQueen(state, m);
    case B_KING:   return CheckBlackKing(state, m);
    case W_KING:   return CheckWhiteKing(state, m);
    default:
        return false;
    }
}