typedef unsigned int uint;
typedef struct
{
    char **board;
} board;
typedef struct
{
    char piece;
    char position;
    bool captures;
    int nub;
} mouve;

board CreateNullBoard(void);
void FreeBoard(board b);

board InitiliseBoard(board b);

void PrintBoard(board b);
mouve getMouve(void);
board Mouve(board b, char piece, uint x, uint y);
bool CheckMouve(board b, char piece, uint x, uint y);