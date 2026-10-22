#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bord.h"

char CHESS_PIECES[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

char **CreateNullBoard(void) {
    // Outer malloc: array of 8 row pointers (char *)
    char **board = malloc(8 * sizeof(char *));
    if (board == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    // Inner malloc: each row points to an array of 8 chars
    for (unsigned int i = 0; i < 8; i++) {
        board[i] = malloc(8 * sizeof(char));
        if (board[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            // Free any previously allocated rows before exiting
            for (unsigned int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            exit(EXIT_FAILURE);
        }
    }
    return board;
}

void FreeBoard(char **board) {
    if (board == NULL) return;

    for (int i = 0; i < 8; i++) {
        free(board[i]);
    }
    free(board);
}

char **InitiliseBoard(char **board) {
    if (board == NULL) {
        fprintf(stderr, "giving a null pointer\n");
        exit(EXIT_FAILURE);
    }

    // Set empty squares
    for (unsigned int i = 2; i < 6; i++) {
        for (unsigned int k = 0; k < 8; k++) {
            board[i][k] = ' ';
        }
    }

    // Set pawns
    for (unsigned int i = 0; i < 8; i++) {
        board[1][i] = 'P';
        board[6][i] = 'p';
    }

    // Set Rooks
    board[0][0] = 'R';
    board[0][7] = 'R';
    board[7][0] = 'r';
    board[7][7] = 'r';

    // Set Knights
    board[0][1] = 'N';
    board[0][6] = 'N';
    board[7][1] = 'n';
    board[7][6] = 'n';

    // Set Bishops
    board[0][2] = 'B';
    board[0][5] = 'B';
    board[7][2] = 'b';
    board[7][5] = 'b';

    // Set Queens
    board[0][3] = 'Q';
    board[7][3] = 'q';

    // Set Kings
    board[0][4] = 'K';
    board[7][4] = 'k';

    return board;
}

void PrintBoard(char **board) {
    printf("         A   B   C   D   E   F   G   H  \n\n");
    for (unsigned int i = 0; i < 8; i++) {
        printf("%u      |", i + 1);
        for (unsigned int k = 0; k < 8; k++, printf("|")) {
            printf(" %c ", board[i][k]);
        }
        printf("\n");
    }
}

mouve *getMouve(void) {
    mouve *curent = malloc(sizeof(mouve));
    if (curent == NULL) return NULL;

    printf("Enter values start end(x9x9): ");
    if (scanf(" %c %u %c %u", &(curent->xStart), &(curent->yStart), &(curent->xEnd), &(curent->yEnd)) == 4) {
        printf("Read successfully: '%c', %u, '%c', %u\n", curent->xStart, curent->yStart, curent->xEnd, curent->yEnd);
    } else {
        printf("Invalid input format.\n");
        free(curent);
        return NULL;
    }
    return curent;
}

void printmouve(char **board, mouve *m) {
    printf("your star position is %u,%c(%c)\n",m->xStart,m->yStart,board[m->xStart][m->yStart]);
    printf("your end position is %u,%c(%c)\n",m->xEnd,m->yEnd,board[m->xEnd][m->yEnd]);
}

char **Mouve(char **board, char piece, unsigned int x, unsigned int y);
bool CheckMouve(char **board, char piece, unsigned int x, unsigned int y);

int main(void) {
    char **b = CreateNullBoard();
    b = InitiliseBoard(b);
    PrintBoard(b);
    mouve * mv =getMouve();
    printmouve(b,mv);
    FreeBoard(b);
    return 0;
}