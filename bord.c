#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bord.h"

char CHESS_PIECES[12] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

/*
 * CHESS PIECES EXPLANATION
 *
 * In standard chess programming, uppercase letters represent White pieces,
 * and lowercase letters represent Black pieces.
 *
 * --- White Pieces ---
 * 'P' : White Pawn   - Moves forward 1 square (or 2 on its first move). Captures 1 square diagonally.
 * 'N' : White Knight - Moves in an "L" shape (2 by 1). The only piece that can jump over others.
 * 'B' : White Bishop - Moves diagonally any number of unobstructed squares.
 * 'R' : White Rook   - Moves horizontally or vertically any number of unobstructed squares.
 * 'Q' : White Queen  - Moves in any direction (horizontal, vertical, diagonal) across unobstructed squares.
 * 'K' : White King   - Moves exactly 1 square in any direction. The game ends if this piece is checkmated.
 *
 * --- Black Pieces ---
 * 'p' : Black Pawn   - Same movement as White Pawn, but moves in the opposite direction.
 * 'n' : Black Knight - Same movement as White Knight.
 * 'b' : Black Bishop - Same movement as White Bishop.
 * 'r' : Black Rook   - Same movement as White Rook.
 * 'q' : Black Queen  - Same movement as White Queen.
 *
 * Standard Starting Layout (Ranks 1 to 8):
 *
 *        A   B   C   D   E   F   G   H
 * 1    [ R , N , B , Q , K , B , N , R ]  (White Major Pieces)
 * 2    [ P , P , P , P , P , P , P , P ]  (White Pawns)
 * 3    [ . , . , . , . , . , . , . , . ]
 * 4    [ . , . , . , . , . , . , . , . ]
 * 5    [ . , . , . , . , . , . , . , . ]
 * 6    [ . , . , . , . , . , . , . , . ]
 * 7    [ p , p , p , p , p , p , p , p ]  (Black Pawns)
 * 8    [ r , n , b , q , k , b , n , r ]  (Black Major Pieces)
 */

board CreateNullBoard(void)
{
    board b;
    b.board = malloc(8 * sizeof(char *));
    if (b.board == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (uint i = 0; i < 8; i++)
    {
        b.board[i] = malloc(8 * sizeof(char));
        if (b.board[i] == NULL)
        {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    return b;
}
void FreeBoard(board b)
{
    for (int i = 0; i < 8; i++)
    {
        free(b.board[i]);
    }
    free(b.board);
}

board InitiliseBoard(board b)
{
    if (b.board == NULL)
    {
        fprintf(stderr, "giving a null pointer\n");
        exit(EXIT_FAILURE);
    }
    // set empty
    for (uint i = 2; i < 6; i++)
    {
        for (uint k = 0; k < 8; k++)
        {
            b.board[i][k] = ' ';
        }
    }
    // set powns
    for (uint i = 0; i < 8; i++)
    {
        b.board[1][i] = 'P';
        b.board[6][i] = 'p';
    }
    // set Rook
    b.board[0][0] = 'R';
    b.board[0][7] = 'R';
    b.board[7][0] = 'r';
    b.board[7][7] = 'r';

    // set Knight
    b.board[0][1] = 'N';
    b.board[0][6] = 'N';
    b.board[7][1] = 'n';
    b.board[7][6] = 'n';

    // set Bishop
    b.board[0][2] = 'B';
    b.board[0][5] = 'B';
    b.board[7][2] = 'b';
    b.board[7][5] = 'b';

    // set Queen
    b.board[0][3] = 'Q';
    b.board[7][3] = 'q';

    // set King
    b.board[0][4] = 'K';
    b.board[7][4] = 'k';
    return b;
}

void PrintBoard(board b)
{
    printf("         A   B   C   D   E   F   G   H  \n\n");
    for (uint i = 0; i < 8; i++)
    {

        printf("%d      |", i + 1);
        for (uint k = 0; k < 8; k++, printf("|"))
        {
            printf(" %c ", b.board[i][k]);
        }
        printf("\n");
    }
}

int main(void)
{
    board b = CreateNullBoard();
    b = InitiliseBoard(b);
    PrintBoard(b);
    FreeBoard(b);
}