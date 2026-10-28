#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "bord.h"
#include "check.h"
#include "mouvesStockfish.h"
uint chooseFlag(char *word,char *flags[],uint sizeFlags){
    if (word==NULL || flags==NULL)return false;
    for(uint i=0;i<sizeFlags;i++){
        int res=strcmp(word,flags[i]);
        if (res==0) return i;
    }
    return UINT_MAX;
}

void startNormal(){
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
}
int main(int argc,char *argv[]) {
    if (argc!=2){
        fprintf(stderr,"you forgot a flag try -h to see all of them\n");
        exit(EXIT_FAILURE);
    }

    char *listFlags[]={"-n","-s","-h"};
    uint res= chooseFlag(argv[1],listFlags,3);
    if (res==0){
        startNormal();
    }else if (res==1){
        startStockfish();
    }else if (res==2){
        printf("add -n for a normal game\nadd -s for a game aguinste stockfish\nadd -h to see this manu again\n");
    }
    
    return EXIT_SUCCESS;
}