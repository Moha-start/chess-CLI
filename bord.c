#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bord.h"

uint **CreateNullBoard(void)
{
    // Outer malloc: array of 8 row pointers (char *)
    uint **board = malloc(8 * sizeof(uint *));
    if (board == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    // Inner malloc: each row points to an array of 8 chars
    for (unsigned int i = 0; i < 8; i++)
    {
        board[i] = malloc(8 * sizeof(uint));
        if (board[i] == NULL)
        {
            fprintf(stderr, "Out of memory\n");
            // Free any previously allocated rows before exiting
            for (unsigned int j = 0; j < i; j++)
            {
                free(board[j]);
            }
            free(board);
            exit(EXIT_FAILURE);
        }
    }
    return board;
}

void FreeBoard(uint **board)
{
    if (board == NULL)
        return;

    for (int i = 0; i < 8; i++)
    {
        free(board[i]);
    }
    free(board);
}

uint **InitiliseBoard(uint **board)
{
    if (board == NULL)
    {
        fprintf(stderr, "giving a null pointer\n");
        exit(EXIT_FAILURE);
    }

    // Set empty squares
    for (unsigned int i = 2; i < 6; i++)
    {
        for (unsigned int k = 0; k < 8; k++)
        {
            board[i][k] = EMPTY;
        }
    }

    // Set pawns
    for (unsigned int i = 0; i < 8; i++)
    {
        board[1][i] = B_PAWN;
        board[6][i] = W_PAWN;
    }

    // Set Rooks
    board[0][0] = B_ROOK;
    board[0][7] = B_ROOK;
    board[7][0] = W_ROOK;
    board[7][7] = W_ROOK;

    // Set Knights
    board[0][1] = B_KNIGHT;
    board[0][6] = B_KNIGHT;
    board[7][1] = W_KNIGHT;
    board[7][6] = W_KNIGHT;

    // Set Bishops
    board[0][2] = B_BISHOP;
    board[0][5] = B_BISHOP;
    board[7][2] = W_BISHOP;
    board[7][5] = W_BISHOP;

    // Set Queens
    board[0][3] = B_QUEEN;
    board[7][3] = W_QUEEN;

    // Set Kings
    board[0][4] = B_KING;
    board[7][4] = W_KING;

    return board;
}

char PieceToChar(uint piece)
{
    switch (piece)
    {
    case EMPTY:
        return '.';
    case B_PAWN:
        return 'p';
    case B_KNIGHT:
        return 'n';
    case B_BISHOP:
        return 'b';
    case B_ROOK:
        return 'r';
    case B_QUEEN:
        return 'q';
    case B_KING:
        return 'k';
    case W_PAWN:
        return 'P';
    case W_KNIGHT:
        return 'N';
    case W_BISHOP:
        return 'B';
    case W_ROOK:
        return 'R';
    case W_QUEEN:
        return 'Q';
    case W_KING:
        return 'K';
    default:
        return '.';
    }
}
void PrintBoard(uint **board)
{
    printf("\n   A   B   C   D   E   F   G   H \n");
    printf(" +---+---+---+---+---+---+---+---+\n");
    for (uint i = 0; i < 8; i++)
    {
        printf("%d |", 8 - i);
        for (unsigned int k = 0; k < 8; k++, printf("|"))
        {
            printf(" %c |", PieceToChar(board[i][k]));
        }
        printf("%d |\n", 8 - i);
        printf(" +---+---+---+---+---+---+---+---+\n");
    }
    printf("\n   A   B   C   D   E   F   G   H \n");
}

mouve *getMouve(void)
{
    mouve *curent = malloc(sizeof(mouve));
    if (curent == NULL)
        return NULL;

    printf("Enter values start end(x9x9): ");
    if (scanf(" %c %u %c %u", &(curent->xStart), &(curent->yStart), &(curent->xEnd), &(curent->yEnd)) == 4)
    {
        printf("Read successfully: '%c', %u, '%c', %u\n", curent->xStart, curent->yStart, curent->xEnd, curent->yEnd);
    }
    else
    {
        printf("Invalid input format.\n");
        free(curent);
        return NULL;
    }
    return curent;
}

void printmouve(uint **board, mouve *m)
{
    printf("your star position is %u,%c(%c)\n", m->xStart, m->yStart, board[m->xStart][m->yStart]);
    printf("your end position is %u,%c(%c)\n", m->xEnd, m->yEnd, board[m->xEnd][m->yEnd]);
}

uint **Mouve(uint **board, uint piece, unsigned int x, unsigned int y);
bool CheckMouve(uint **board, uint piece, unsigned int x, unsigned int y);

int main(void)
{
    uint **b = CreateNullBoard();
    b = InitiliseBoard(b);
    PrintBoard(b);
    mouve *mv = getMouve();
    printmouve(b, mv);
    FreeBoard(b);
    return 0;
}