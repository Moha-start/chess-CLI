#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include "bord.h"
#include "check.h"
uint chooseFlag(char *word,char *flags[],uint sizeFlags){
    if (word==NULL || flags==NULL)return false;
    for(uint i=0;i<sizeFlags;i++){
        int res=strcmp(word,flags[i]);
        if (res==0) return i;
    }
    return UINT_MAX;
}
void startStockfish(void){
    int pipeIn[2];
    int pipeOut[2];
    if (pipe(pipeIn) == -1 || pipe(pipeOut) == -1){
        perror("Erreur lors de la création des pipes");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if (pid == -1){
        perror("Erreur lors du fork");
         exit(EXIT_FAILURE);
    }
    if (pid == 0){
        dup2(pipeIn[0],STDIN_FILENO);
        dup2(pipeOut[0],STDOUT_FILENO);
        close(pipeIn[0]);close(pipeIn[1]);
        close(pipeOut[0]);close(pipeOut[1]);
        execlp("stockfish", "stockfish", NULL);
        perror("Erreur au lancement de Stockfish");
         exit(EXIT_FAILURE);
    } else {
        close(pipeIn[0]);
        close(pipeOut[1]);
        #define SEND(cmd) write(pipeIn[1], cmd, strlen(cmd))
        SEND("uci\n");
        SEND("isready\n");
        //get my fist mouve
        SEND("position startpos moves e2e4 e7e5 g1f3\n");
        SEND("go movetime 1000\n");
        char buffer[4096];
        ssize_t byteRead;
        char *bestmouvePtr= NULL;
        while ((byteRead=read(pipeOut[0], buffer, sizeof(buffer) -1 )) > 0){
            buffer[byteRead]='\0';
            bestmouvePtr=strstr(buffer,"bestmove");
            if (bestmouvePtr != NULL){
                printf("Résultat intercepté : %s\n", bestmouvePtr);
                break;
            }

        }
        SEND("quit\n");
        close(pipeIn[1]);
        close(pipeOut[0]);
        wait(NULL);
    }
    
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