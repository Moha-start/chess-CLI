typedef unsigned int uint;
typedef struct
{
    char **board;
} board;

board CreateNullBoard(void);
void FreeBoard(board b);

board InitiliseBoard(board b);

void PrintBoard(board b);
board Mouve(board b, char piece, uint x, uint y);
bool CheckMouve(board b, char piece, uint x, uint y);