#ifndef BORD_H
#define BORD_H

/** Number of files and ranks on a standard chess board. */
#define BOARDSIZE 8

/** Maximum number of plies reserved for move history. */
#define MAX_HISTORY_PLIES 2048
#include <stdbool.h>


/**
 * Piece identifiers used in board squares.
 *
 * EMPTY represents a vacant square. PAWN through KING are uncolored piece
 * ranks; white pieces have identifiers 11-16 and black pieces 21-26.
 */
enum pieces
{
    EMPTY = 0,
    PAWN ,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    W_PAWN = 11,
    W_KNIGHT,
    W_BISHOP,
    W_ROOK,
    W_QUEEN,
    W_KING,
    B_PAWN = 21,
    B_KNIGHT,
    B_BISHOP,
    B_ROOK,
    B_QUEEN,
    B_KING,
};

/** Unsigned integer type used for coordinates and piece identifiers. */
typedef unsigned int uint;

/**
 * A move between two board squares.
 *
 * Coordinates are zero-based: x is the file (0=A through 7=H), and y is the
 * grid row (0=rank 8 through 7=rank 1). isWhite indicates the moving side.
 */
typedef struct{
    uint xStart;
    uint yStart;
    uint xEnd;
    uint yEnd;
    bool isWhite;
} mouve;

/**
 * The board and state tracked during a game.
 *
 * grid is an 8x8 array of piece identifiers. KingMouved and rookmouved track
 * castling-related movement. emptyPieces stores captured-piece counts by
 * color (0=white, 1=black) and piece rank. oldMouves stores move history.
 */
typedef struct {
    bool KingMouved[2];
    bool rookmouved[2][2];
    bool hasLastMove;
    uint Indexmouve;
    mouve lastMovePlayed;
    uint emptyPieces[2][13];
    uint **grid;
    char *oldMouves;
    
} GameState;



/**
 * Allocate a game state with storage for its board and move history.
 *
 * @return Pointer to the allocated state; terminates the process on allocation
 *         failure. The board position is not initialized.
 */
GameState *CreateNullBoard(void);

/**
 * Release a game state and the storage it owns.
 *
 * @param state State to free; NULL is accepted.
 * @return Nothing.
 */
void FreeBoard(GameState *state);

/**
 * Set the board squares to the standard chess starting position.
 *
 * @param state State whose grid will be initialized; NULL terminates the
 *              process.
 * @return Nothing.
 */
void InitiliseBoard(GameState *state);

/**
 * Convert a piece identifier to its display character.
 *
 * @param piece Piece identifier, such as W_PAWN or B_KING.
 * @return Character for the piece, '.' for EMPTY or an unknown identifier.
 */
char PieceToChar(uint piece);

/**
 * Print the board to standard output.
 *
 * @param state Board state to print.
 * @return Nothing.
 */
void PrintBoard(GameState *state);

/**
 * Convert a file letter to its zero-based file index.
 *
 * @param c File letter A-H or a-h.
 * @return Index from 0 to 7, or 99 if c is not a valid file letter.
 */
uint FromChartoUint(char c);

/**
 * Convert a zero-based file index to a file letter.
 *
 * @param n File index from 0 to 7.
 * @param maj If true, return an uppercase letter; otherwise, lowercase.
 * @return File letter, or '\0' if n is greater than 7.
 */
char FromUintToChar(uint n,bool maj);

/**
 * Create a new string by inserting text into another string.
 *
 * @param str1 Original string whose prefix is retained.
 * @param str2 Text inserted after the prefix.
 * @param index Number of characters from str1 to retain before insertion;
 *              values greater than the length of str1 are clamped to its
 *              length.
 * @return Newly allocated string containing the retained prefix, str2, and a
 *         trailing newline. The caller must free the returned string.
 */
char *combineStr(const char *str1, const char *str2, unsigned int index);

/**
 * Prompt for and read a move for the specified side.
 *
 * Input is expected as a start file, start rank, end file, and end rank
 * (for example, "e 2 e 4").
 *
 * @param isWhite true when the move is for white, false for black.
 * @return Allocated move, which the caller must free; NULL if the input format
 *         is invalid or allocation fails. Invalid coordinates terminate the
 *         process.
 */
mouve *getMouve(bool isWhite);

/**
 * Print a move's start and end squares and the pieces currently on them.
 *
 * @param state Board state used to look up the pieces.
 * @param m Move whose coordinates will be printed.
 * @return Nothing.
 */
void printmouve(GameState *state, mouve *m);

/**
 * Validate and apply a move, updating the board and move-history state.
 *
 * @param state Game state to validate against and update.
 * @param m Move to validate and apply.
 * @return true if the move is accepted and applied; false if rejected.
 */
bool Mouve(GameState *state, mouve *m);

#endif // BORD_H