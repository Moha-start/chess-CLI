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

/*
 *1. The Piece (Character 1)

   *The first character is always a capital letter representing the piece being moved.
    *K = King
    *Q = Queen
    *R = Rook
    *B = Bishop
    *N = Knight (since 'K' is taken by the King)
    *(Pawns do not get a letter—you just write the square they move to).

 *2. The Destination Square (Characters 2 & 3)

  *The next two characters are the exact coordinate of the square the piece is landing on.

    *Column (File): A lowercase letter from a to h.
    *Row (Rank): A number from 1 to 8.

Examples of 3-Character Moves

    *Nf3 : Nknight moves to square f3.
    *Bc4 : Bishop moves to square c4.
    *Qd8 : Queen moves to square d8.
    *O-O : Kingside Castling (also exactly 3 characters).

 *The Exceptions (When it's not 3 characters)

    *Pawn moves (2 chars): Since pawns don't use a capital letter, moving a pawn to e4 is simply written as e4.
    *Captures (4 chars): If a piece is captured, an 'x' is inserted in the middle. E.g., Nxd4 (Knight captures on d4).
    *Disambiguation (4 chars): If two of the same pieces can move to the same square (like two Rooks), you specify which one moved by adding its starting column. E.g., Rad1 (The Rook on the 'a' file moves to d1).*/
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
mouve getMouve(void)
{
    char piece;
    char position;
    bool captures;
    uint nub;
    mouve *curent = malloc(sizeof(mouve));
    scanf("%d");
}
board Mouve(board b, char piece, uint x, uint y);
bool CheckMouve(board b, char piece, uint x, uint y);
int main(void)
{
    board b = CreateNullBoard();
    b = InitiliseBoard(b);
    PrintBoard(b);
    FreeBoard(b);
}