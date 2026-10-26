#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include "bord.h"

/**
 * Returns true if the square at (x, y) is empty.
 *
 * @param board The chess board to inspect.
 * @param x The horizontal coordinate of the square.
 * @param y The vertical coordinate of the square.
 * @return true if the square contains no piece; otherwise false.
 */
bool isEmpty(GameState *board, uint x, uint y);

/**
 * Determines whether the piece on the square at (x, y) belongs to White.
 *
 * This function expects the square to contain a piece. If the square is empty,
 * it prints an error and exits the program.
 *
 * @param board The chess board to inspect.
 * @param x The horizontal coordinate of the square.
 * @param y The vertical coordinate of the square.
 * @return true if the piece is white; false if it is black.
 */
bool isWhite(GameState *board, uint x, uint y);

/**
 * Checks whether the king located at (x, y) is currently under attack.
 *
 * The attack detection considers adjacent kings, pawns, knights, rook/queen
 * lines, and bishop/queen diagonals for the opponent's side.
 *
 * @param board The chess board to inspect.
 * @param x The king's x coordinate.
 * @param y The king's y coordinate.
 * @param isWhiteKing true if the king being checked is white, false otherwise.
 * @return true if the king is in check; false otherwise.
 */
bool isKingAttaked(GameState *board, uint x, uint y, bool isWhiteKing);

/**
 * Validates a white pawn move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows white pawn rules; false otherwise.
 */
bool CheckWhitePawn(GameState *board, mouve *m);

/**
 * Validates a black pawn move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows black pawn rules; false otherwise.
 */
bool CheckBlackPawn(GameState *board, mouve *m);

/**
 * Validates a white knight move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows white knight rules; false otherwise.
 */
bool CheckWhiteKnight(GameState *board, mouve *m);

/**
 * Validates a black knight move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows black knight rules; false otherwise.
 */
bool CheckBlackKnight(GameState *board, mouve *m);

/**
 * Validates a white bishop move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows white bishop rules; false otherwise.
 */
bool CheckWhiteBishop(GameState *board, mouve *m);

/**
 * Validates a black bishop move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows black bishop rules; false otherwise.
 */
bool CheckBlackBishop(GameState *board, mouve *m);

/**
 * Validates a white rook move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows white rook rules; false otherwise.
 */
bool CheckWhiteRook(GameState *board, mouve *m);

/**
 * Validates a black rook move described by m.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows black rook rules; false otherwise.
 */
bool CheckBlackRook(GameState *board, mouve *m);

/**
 * Validates a white queen move described by m.
 *
 * A queen is allowed to move like a rook or a bishop.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows white queen rules; false otherwise.
 */
bool CheckWhiteQueen(GameState *board, mouve *m);

/**
 * Validates a black queen move described by m.
 *
 * A queen is allowed to move like a rook or a bishop.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows black queen rules; false otherwise.
 */
bool CheckBlackQueen(GameState *board, mouve *m);

/**
 * Validates a white king move described by m.
 *
 * A king may move one square in any direction, provided the destination is not
 * under attack by the opponent.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows white king rules; false otherwise.
 */
bool CheckWhiteKing(GameState *board, mouve *m);

/**
 * Validates a black king move described by m.
 *
 * A king may move one square in any direction, provided the destination is not
 * under attack by the opponent.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure containing start and end coordinates.
 * @return true if the move follows black king rules; false otherwise.
 */
bool CheckBlackKing(GameState *board, mouve *m);

/**
 * Validates a move for the piece currently located at the starting square.
 *
 * The function checks the piece type, verifies that the move matches the color
 * of the player, and delegates to the appropriate piece-specific validation.
 *
 * @param board The chess board.
 * @param m Pointer to the move structure describing the attempted move.
 * @return true if the move is legal for the piece and color; false otherwise.
 */
bool CheckMouve(GameState *board, mouve *m);

#endif // CHECK_H