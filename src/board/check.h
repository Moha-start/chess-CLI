#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include "bord.h"

/**
 * Returns the rank value stored in a state cell.
 *
 * @param state Pointer to the current game state.
 * @param x Column index of the square.
 * @param y Row index of the square.
 * @return Piece rank in the range 0..9, or 0 for an empty square.
 */
uint getRank(GameState *state, uint x, uint y);

/**
 * Extracts the rank portion of a packed chess piece value.
 *
 * @param piece Encoded piece value from the state grid.
 * @return Piece rank without its color byte.
 */
uint getRankPiece(uint piece);

/**
 * Validates if the attempted move is a legal en passant capture.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if it is a valid en passant, false otherwise.
 */
bool isEnPassant(GameState *state, mouve *m);

/**
 * get the position of bouth kings
 *
 * @param state Pointer to the current game state.
 * @param coord Pointer to a 2D list that will contain the cordinate for the kings
 */
void getKings(GameState *state,uint * coord[]);

/**
 * Tests whether a square is empty.
 *
 * @param state Pointer to the current game state.
 * @param x Column index of the square.
 * @param y Row index of the square.
 * @return true if the target square contains no piece.
 */
bool isEmpty(GameState *state, uint x, uint y);

/**
 * Determines whether the piece on a square belongs to White.
 *
 * @param state Pointer to the current game state.
 * @param x Column index of the square.
 * @param y Row index of the square.
 * @return true if the square contains a white piece.
 */
bool isWhite(GameState *state, uint x, uint y);

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
 * @param state Pointer to the current game state.
 * @param x Column index of the king.
 * @param y Row index of the king.
 * @param isWhiteKing true when validating a white king; false for a black king.
 * @return true if the king is in check.
 */
bool isKingAttaked(GameState *state, uint x, uint y, bool isWhiteKing);

/**
 * Validates a legal white pawn move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white pawn.
 */
bool CheckWhitePawn(GameState *state, mouve *m);

/**
 * Validates a legal black pawn move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black pawn.
 */
bool CheckBlackPawn(GameState *state, mouve *m);

/**
 * Validates a legal white knight move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white knight.
 */
bool CheckWhiteKnight(GameState *state, mouve *m);

/**
 * Validates a legal black knight move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black knight.
 */
bool CheckBlackKnight(GameState *state, mouve *m);

/**
 * Validates a legal white bishop move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white bishop.
 */
bool CheckWhiteBishop(GameState *state, mouve *m);

/**
 * Validates a legal black bishop move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black bishop.
 */
bool CheckBlackBishop(GameState *state, mouve *m);

/**
 * Validates a legal white rook move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white rook.
 */
bool CheckWhiteRook(GameState *state, mouve *m);

/**
 * Validates a legal black rook move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black rook.
 */
bool CheckBlackRook(GameState *state, mouve *m);

/**
 * Validates a legal white queen move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white queen.
 */
bool CheckWhiteQueen(GameState *state, mouve *m);

/**
 * Validates a legal black queen move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black queen.
 */
bool CheckBlackQueen(GameState *state, mouve *m);

/**
 * Validates a legal white king move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a white king.
 */
bool CheckWhiteKing(GameState *state, mouve *m);

/**
 * Validates a legal black king move for the given move descriptor.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the move matches the rules for a black king.
 */
bool CheckBlackKing(GameState *state, mouve *m);

/**
 * Checks whether a move is legal according to the state rules.
 *
 * @param state Pointer to the current game state.
 * @param m Move description to validate.
 * @return true if the supplied move is valid for the current position.
 */
bool CheckMouve(GameState *state, mouve *m);

#endif // CHECK_H