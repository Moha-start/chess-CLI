#include <stdio.h>
#include <stdlib.h>
#include "bord.h"
#include "check.h"

int main(void) {
    bool isWhite = true;
    Board b = CreateNullBoard();
    InitiliseBoard(b);
    PrintBoard(b);
    mouve *mv = getMouve(isWhite);
    if (mv == NULL) {
        fprintf(stderr, "there is an error with the mouvement\n");
        exit(EXIT_FAILURE);
    }
    printf("Move is valid: %u\n", CheckMouve(b, mv));
    printmouve(b, mv);
    Mouve(b, mv);
    FreeBoard(b);
    return 0;
}