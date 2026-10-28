#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bord.h"
#include "check.h"

GameState *CreateNullBoard(void) {
    GameState *state = malloc(sizeof(*state));
    if (state == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    memset(state->emptyPieces, 0, sizeof(state->emptyPieces));

    state->grid = malloc(8 * sizeof(*state->grid));
    
    if (state->grid == NULL) {
        fprintf(stderr, "Out of memory\n");
        free(state);
        exit(EXIT_FAILURE);
    }
    state->oldMouves = malloc((6 * MAX_HISTORY_PLIES) + 1);
    if (state->oldMouves == NULL) {
        fprintf(stderr, "Out of memory\n");
        free(state->grid);
        free(state);
        exit(EXIT_FAILURE);
    }
    for (uint i = 0; i < 8; i++) {
        state->grid[i] = malloc(8 * sizeof(*state->grid[i]));
        if (state->grid[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            for (uint j = 0; j < i; j++) {
                free(state->grid[j]);
            }
            free(state->oldMouves);
            free(state->grid);
            free(state);
            exit(EXIT_FAILURE);
        }
    }
    return state;
}

void FreeBoard(GameState *state) {
    if (state == NULL) return;
    if(state->oldMouves!=NULL){
        free(state->oldMouves);
    }
    if (state->grid != NULL) {
        for (int i = 0; i < 8; i++) {
            free(state->grid[i]);
        }
        free(state->grid);
    }
    
    free(state);
}

void InitiliseBoard(GameState *state) {
    if (state == NULL) {
        fprintf(stderr, "giving a null pointer\n");
        exit(EXIT_FAILURE);
    }
    
    for (unsigned int i = 2; i < 6; i++) {
        for (unsigned int k = 0; k < 8; k++) {
            state->grid[i][k] = EMPTY;
        }
    }

    for (unsigned int i = 0; i < 8; i++) {
        state->grid[1][i] = B_PAWN;
        state->grid[6][i] = W_PAWN;
    }

    state->grid[0][0] = B_ROOK;
    state->grid[0][7] = B_ROOK;
    state->grid[7][0] = W_ROOK;
    state->grid[7][7] = W_ROOK;

    state->grid[0][1] = B_KNIGHT;
    state->grid[0][6] = B_KNIGHT;
    state->grid[7][1] = W_KNIGHT;
    state->grid[7][6] = W_KNIGHT;

    state->grid[0][2] = B_BISHOP;
    state->grid[0][5] = B_BISHOP;
    state->grid[7][2] = W_BISHOP;
    state->grid[7][5] = W_BISHOP;

    state->grid[0][3] = B_QUEEN;
    state->grid[7][3] = W_QUEEN;

    state->grid[0][4] = B_KING;
    state->grid[7][4] = W_KING;
    state->KingMouved[0] = false;
    state->KingMouved[1] = false;
    state->rookmouved[0][0]=false;
    state->rookmouved[0][1]=false;
    state->rookmouved[1][0]=false;
    state->rookmouved[1][1]=false;
}

char PieceToChar(uint piece) {
    switch (piece) {
    case EMPTY: return '.';
    case B_PAWN: return 'p';
    case B_KNIGHT: return 'n';
    case B_BISHOP: return 'b';
    case B_ROOK: return 'r';
    case B_QUEEN: return 'q';
    case B_KING: return 'k';
    case W_PAWN: return 'P';
    case W_KNIGHT: return 'N';
    case W_BISHOP: return 'B';
    case W_ROOK: return 'R';
    case W_QUEEN: return 'Q';
    case W_KING: return 'K';
    default: return '.';
    }
}

void PrintBoard(GameState *state) {
    printf("\n    A   B   C   D   E   F   G   H \n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (uint i = 0; i < 8; i++) {
        printf("%d |", 8 - i);
        for (unsigned int k = 0; k < 8; k++) {
            printf(" %c |", PieceToChar(state->grid[i][k]));
        }
        printf(" %d\n", 8 - i);
    }
    printf("  +---+---+---+---+---+---+---+---+\n");
    printf("    A   B   C   D   E   F   G   H \n");
}

uint FromChartoUint(char c) {
    if (c == 'A' || c == 'a') return 0;
    else if (c == 'B' || c == 'b') return 1;
    else if (c == 'C' || c == 'c') return 2;
    else if (c == 'D' || c == 'd') return 3;
    else if (c == 'E' || c == 'e') return 4;
    else if (c == 'F' || c == 'f') return 5;
    else if (c == 'G' || c == 'g') return 6;
    else if (c == 'H' || c == 'h') return 7;
    return 99;
}

char FromUintToChar(unsigned int n, bool maj) {
    if (n > 7) {
        return '\0'; 
    }
    
    char base = maj ? 'A' : 'a';
    return (char)(base + n);
}

mouve *getMouve(bool isWhite) {
    mouve *curent = malloc(sizeof(mouve));
    if (curent == NULL) return NULL;
    char xS, xE;
    uint yS, yE;
    
    printf("Enter values start end(x9x9): ");
    if (scanf(" %c %u %c %u", &xS, &yS, &xE, &yE) == 4) {
        uint xstart = FromChartoUint(xS);
        uint xend = FromChartoUint(xE);
        if (xend > 7 || xstart > 7 || yS < 1 || yS > 8 || yE < 1 || yE > 8) {
            fprintf(stderr, "you must give a valide piece\n");
            exit(EXIT_FAILURE);
        }
        curent->xStart = xstart; 
        curent->xEnd = xend;
        curent->yStart = 8 - yS;
        curent->yEnd = 8 - yE;
    } else {
        printf("Invalid input format.\n");
        free(curent);
        return NULL;
    }
    curent->isWhite = isWhite;
    return curent;
}

void printmouve(GameState *state, mouve *m) {
    printf("your star position is %c,%u(%c)\n", 
            FromUintToChar(m->xStart,true), 8 - m->yStart, PieceToChar(state->grid[m->yStart][m->xStart]));
            
    printf("your end position is %c,%u(%c)\n", 
            FromUintToChar(m->xEnd,true), 8 - m->yEnd, PieceToChar(state->grid[m->yEnd][m->xEnd]));
}

void saveMouve(GameState *state, mouve *m) {
    uint index = state->Indexmouve;
    
    state->oldMouves[index] = FromUintToChar(m->xStart,false);    
    state->oldMouves[index+1] = (8 - m->yStart) + '0';
    
    state->oldMouves[index+2] = FromUintToChar(m->xEnd,false);
    state->oldMouves[index+3] = (8 - m->yEnd) + '0';
    
    state->oldMouves[index+4] = ' ';
    state->Indexmouve += 5;
}
bool Mouve(GameState *state, mouve *m) {
    if (!CheckMouve(state, m)) return false;
    
    uint start = state->grid[m->yStart][m->xStart];
    uint dest = state->grid[m->yEnd][m->xEnd];
    
    bool isEnPassantCapture = false;
    uint capturedPieceEnPassant = EMPTY;
    if (getRankPiece(start) == PAWN && m->xStart != m->xEnd && isEmpty(state, m->xEnd, m->yEnd)) {
        isEnPassantCapture = true;
        capturedPieceEnPassant = state->grid[m->yStart][m->xEnd];
    }

    if (!isEmpty(state, m->xEnd, m->yEnd)){
        uint color = isWhitePiece(dest) ? 0 : 1;
        uint piece = getRankPiece(dest);
        state->emptyPieces[color][piece]++;
    } else if (isEnPassantCapture) {
        uint color = isWhitePiece(capturedPieceEnPassant) ? 0 : 1;
        uint piece = getRankPiece(capturedPieceEnPassant);
        state->emptyPieces[color][piece]++;
        
        state->grid[m->yStart][m->xEnd] = EMPTY;
    }
    
    uint color = isWhite(state, m->xStart, m->yStart) ? 0 : 1;
    if (getRankPiece(start) == KING && !state->KingMouved[color]) state->KingMouved[color] = true;
    if (getRankPiece(start) == ROOK){
        uint pos = m->xStart / 7;
        if (!state->rookmouved[color][pos]) state->rookmouved[color][pos] = true;
    }

    state->grid[m->yEnd][m->xEnd] = start;
    state->grid[m->yStart][m->xStart] = EMPTY;
    
    state->lastMovePlayed = *m;
    state->hasLastMove = true;
    saveMouve(state,m);

    
    return true; 
}
