#ifndef BORD_H
#define BORD_H

#include <stdbool.h>

typedef unsigned int uint;

// Represent the board directly as a 2D dynamically allocated array
typedef char** Board;

typedef struct {
    char xStart;
    uint yStart;
    char xEnd;
    uint yEnd;
} mouve;

// Board allocation and lifecycle
char **CreateNullBoard(void);
void FreeBoard(char **board);
char **InitiliseBoard(char **board);
void PrintBoard(char **board);

// Move operations
mouve *getMouve(void);
void printmouve(char **board, mouve *m);
char **Mouve(char **board, char piece, uint x, uint y);
bool CheckMouve(char **board, char piece, uint x, uint y);

#endif // BORD_H