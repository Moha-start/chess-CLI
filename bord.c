#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bord.h"
#include "check.h"

Board CreateNullBoard(void) {
    uint **board = malloc(8 * sizeof(uint *));
    if (board == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned int i = 0; i < 8; i++) {
        board[i] = malloc(8 * sizeof(uint));
        if (board[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            for (unsigned int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            exit(EXIT_FAILURE);
        }
    }
    return board;
}

void FreeBoard(Board board) {
    if (board == NULL) return;

    for (int i = 0; i < 8; i++) {
        free(board[i]);
    }
    free(board);
}

void InitiliseBoard(Board board) {
    if (board == NULL) {
        fprintf(stderr, "giving a null pointer\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned int i = 2; i < 6; i++) {
        for (unsigned int k = 0; k < 8; k++) {
            board[i][k] = EMPTY;
        }
    }

    for (unsigned int i = 0; i < 8; i++) {
        board[1][i] = B_PAWN;
        board[6][i] = W_PAWN;
    }

    board[0][0] = B_ROOK;
    board[0][7] = B_ROOK;
    board[7][0] = W_ROOK;
    board[7][7] = W_ROOK;

    board[0][1] = B_KNIGHT;
    board[0][6] = B_KNIGHT;
    board[7][1] = W_KNIGHT;
    board[7][6] = W_KNIGHT;

    board[0][2] = B_BISHOP;
    board[0][5] = B_BISHOP;
    board[7][2] = W_BISHOP;
    board[7][5] = W_BISHOP;

    board[0][3] = B_QUEEN;
    board[7][3] = W_QUEEN;

    board[0][4] = B_KING;
    board[7][4] = W_KING;
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

void PrintBoard(Board board) {
    printf("\n    A   B   C   D   E   F   G   H \n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (uint i = 0; i < 8; i++) {
        printf("%d |", 8 - i);
        for (unsigned int k = 0; k < 8; k++) {
            printf(" %c |", PieceToChar(board[i][k]));
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

char FromUintToChar(uint n) {
    char mot[] = "ABCDEFGH";
    return mot[n];
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

void printmouve(Board board, mouve *m) {
    printf("your star position is %c,%u(%c)\n", 
            FromUintToChar(m->xStart), 8 - m->yStart, PieceToChar(board[m->yStart][m->xStart]));
            
    printf("your end position is %c,%u(%c)\n", 
            FromUintToChar(m->xEnd), 8 - m->yEnd, PieceToChar(board[m->yEnd][m->xEnd]));
}

bool Mouve(Board board, mouve *m) {
    if (!CheckMouve(board, m)) return false;
    return true; 
}

int main(void) {
    bool isWhite = true;
    Board b = CreateNullBoard();
    InitiliseBoard(b);
    PrintBoard(b);
    mouve *mv = getMouve(isWhite);
    if (mv == NULL) {
        fprintf(stderr, "ther is an error with the mouvement\n");
        exit(EXIT_FAILURE);
    }
    printf("Move is valid: %u\n", CheckMouve(b, mv));
    printmouve(b, mv);
    Mouve(b, mv);
    FreeBoard(b);
    return 0;
}