#include <stdio.h>
#include <stdlib.h>
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

board create_null_board(void)
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
void free_board(board b)
{
    for (int i = 0; i < 8; i++)
    {
        free(b.board[i]);
    }
    free(b.board);
}

board initilise_board(board b)
{
    if (b.board == NULL)
    {
        fprintf(stderr, "giving a null pointer\n");
        exit(EXIT_FAILURE);
    }
    for (uint i = 0; i < 8; i++)
    {
        b.board[1][i] = 'P';
        b.board[6][i] = 'p';
    }
}