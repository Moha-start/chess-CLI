#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include "bord.h"

/**
 * Returns the rank value stored in a board cell.
 *
 * @param board Pointer to the current game state.
 * @param x Column index of the square.
 * @param y Row index of the square.
 * @return Piece rank in the range 0..9, or 0 for an empty square.
 */
uint getRank(GameState *board, uint x, uint y);

/**
 * Extracts the rank portion of a packed chess piece value.
 *
 * @param piece Encoded piece value from the board grid.
 * @return Piece rank without its color byte.
 */
uint getRankPiece(uint piece);

/**
 * Tests whether a square is empty.
 *
 * @param board Pointer to the current game state.
 * @param x Column index of the square.
 * @param y Row index of the square.
 * @return true if the target square contains no piece.
 */
bool isEmpty(GameState *board, uint x, uint y);

/**
 * Determines whether the piece on a square belongs to White.
 *
 * @param board Pointer to the current game state.
 * @param x Column index of the square.
 * @param y Row index of the square.
 * @return true if the square contains a white piece.
 */
bool isWhite(GameState *board, uint x, uint y);

/**
 * Determines whether a packed piece value belongs to White.
 *
 * @param piece Encoded piece value.
 * @return true if the piece is white.
 */
bool isWhitePiece(uint piece);

/**
 * Checks whether the king at (x, y) is attacked by an opposing piece.
 *
 * @param board Pointer to the current game state.
 * @param x Column index of the king.
 * @param y Row index of the king.
 * @param isWhiteKing true when validating a white king; false for a black king.
 * @return true if the king is in check.
 */
bool isKingAttaked(GameState *board, uint x, uint y, bool isWhiteKing);

/**
 * Validates a legal white pawn move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white pawn.
 */
bool CheckWhitePawn(GameState *board, mouve *m);

/**
 * Validates a legal black pawn move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black pawn.
 */
bool CheckBlackPawn(GameState *board, mouve *m);

/**
 * Validates a legal white knight move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white knight.
 */
bool CheckWhiteKnight(GameState *board, mouve *m);

/**
 * Validates a legal black knight move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black knight.
 */
bool CheckBlackKnight(GameState *board, mouve *m);

/**
 * Validates a legal white bishop move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white bishop.
 */
bool CheckWhiteBishop(GameState *board, mouve *m);

/**
 * Validates a legal black bishop move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black bishop.
 */
bool CheckBlackBishop(GameState *board, mouve *m);

/**
 * Validates a legal white rook move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white rook.
 */
bool CheckWhiteRook(GameState *board, mouve *m);

/**
 * Validates a legal black rook move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black rook.
 */
bool CheckBlackRook(GameState *board, mouve *m);

/**
 * Validates a legal white queen move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white queen.
 */
bool CheckWhiteQueen(GameState *board, mouve *m);

/**
 * Validates a legal black queen move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black queen.
 */
bool CheckBlackQueen(GameState *board, mouve *m);

/**
 * Validates a legal white king move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white king.
 */
bool CheckWhiteKing(GameState *board, mouve *m);

/**
 * Validates a legal black king move for the given move descriptor.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black king.
 */
bool CheckBlackKing(GameState *board, mouve *m);

/**
 * Checks whether a move is legal according to the board rules.
 *
 * @param board Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the supplied move is valid for the current position.
 */
bool CheckMouve(GameState *board, mouve *m);

#endif // CHECK_H