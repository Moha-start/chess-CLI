typedef unsigned int uint;
typedef struct
{
    char **board;
} board;

board create_null_board(void);
void free_board(board b);

board initilise_board(board b);