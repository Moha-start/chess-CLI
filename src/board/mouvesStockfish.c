#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include "bord.h"
#include "check.h"
#include "mouvesStockfish.h"
mouve *Stockfishmouve(GameState *state, char *bestmouvePtr) {
    mouve *curent = malloc(sizeof(mouve));
    if (curent == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    char xS = bestmouvePtr[9];
    char yS = bestmouvePtr[10];
    char xE = bestmouvePtr[11];
    char yE = bestmouvePtr[12];

    uint xstart = FromChartoUint(xS);
    uint xend = FromChartoUint(xE);
    uint ystart = (uint)(yS - '0');
    uint yend = (uint)(yE - '0');
    
    if (xend > 7 || xstart > 7 || ystart < 1 || ystart > 8 || yend < 1 || yend > 8) {
        fprintf(stderr, "you must give a valide piece\n");
        free(curent);
        exit(EXIT_FAILURE);
    }
    
    curent->xStart = xstart; 
    curent->xEnd = xend;
    curent->yStart = 8 - ystart;
    curent->yEnd = 8 - yend;

    curent->isWhite = false;
    
    return curent;
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
        dup2(pipeOut[1],STDOUT_FILENO);
        close(pipeIn[0]);close(pipeIn[1]);
        close(pipeOut[0]);close(pipeOut[1]);
        //execlp("stockfish", "stockfish", NULL);
        execl("./stockfish", "stockfish", NULL);
        perror("Erreur au lancement de Stockfish");
        exit(EXIT_FAILURE);
    } else {
        GameState *b = CreateNullBoard();
        InitiliseBoard(b);
        bool isPWhite=true;
        close(pipeIn[0]);
        close(pipeOut[1]);
        #define SEND(cmd) write(pipeIn[1], cmd, strlen(cmd))
        SEND("uci\n");
        SEND("isready\n");
        while(!isFiniched(b,isPWhite)){
            if(isPWhite){
                while(true){
                    PrintBoard(b);
                    mouve *mv = getMouve(b,isPWhite);
                    printmouve(b,mv);
                    if (mv == NULL) {
                        FreeBoard(b);
                        fprintf(stderr, "there is an error with the mouvement\n");
                        exit(EXIT_FAILURE);
                    }
                    bool valide = Mouve(b, mv);
                    if (!valide){
                        printf("you must give a valide mouve!");
                        free(mv);
                    }else {
                        isPWhite=false;
                        printf("%d\n",isFiniched(b,isPWhite));
                        break;
                    }
                }
            }else{
                printf("start in here\n");
                char start[]="position startpos moves ";
                char * result = combineStr(start, b->oldMouves, strlen(start));

                SEND(result);
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
                if (bestmouvePtr == NULL) {
                    fprintf(stderr, "Error: Stockfish closed unexpectedly or did not send bestmove.\n");
                    FreeBoard(b);
                    exit(EXIT_FAILURE);
                }
                mouve *mv=Stockfishmouve(b,bestmouvePtr);
                bool valide =Mouve(b,mv);
                if (!valide){
                    fprintf(stderr, "stockfish send wrong mouve\n");
                    free(mv);
                    FreeBoard(b);
                    exit(EXIT_FAILURE);
                }else{
                    isPWhite=true;
                }

            }
        }

        SEND("quit\n");
        close(pipeIn[1]);
        close(pipeOut[0]);
        wait(NULL);
    }
    
}