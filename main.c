#include <stdio.h>
#include <stdlib.h>
#include "bord.h"
#include "check.h"

int main(void) {
    bool isWhite = true;
    GameState *b = CreateNullBoard();
    InitiliseBoard(b);
    PrintBoard(b);
    mouve *mv = getMouve(isWhite);
    if (mv == NULL) {
        fprintf(stderr, "there is an error with the mouvement\n");
        exit(EXIT_FAILURE);
    }
    printmouve(b, mv);
    Mouve(b, mv);
    PrintBoard(b);
    FreeBoard(b);
    return 0;
}