/*
 * piece.h
 *
 *  Created on: Jan 13, 2025
 *      Author: nikutsuki
 */

#ifndef INC_PIECE_H_
#define INC_PIECE_H_

#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define PIECE_SIZE 4

typedef enum {
	LONG = 0,
	L = 1,
	SQUARE = 2,
	S = 3,
	T = 4,
	L_FLIPPED = 5,
	S_FLIPPED = 6
} piece_type;

typedef enum {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
} rotation;

typedef struct piece {
	block_t blocks[4][4];
	piece_type type;
	int x;
	int y;

} piece_t;

piece_t* construct_piece(piece_type type, int x, int y);
void render_piece(piece_t* piece);
void move_piece(piece_t* piece, int x, int y);
void delete_piece(piece_t* piece);
void rotate_piece(piece_t* piece, block_t blocks[10][20]);
bool check_is_rotation_valid(piece_t* piece, block_t blocks[10][20]);

extern const int LONG_PIECE[4][4];
extern const int L_PIECE[4][4];
extern const int L_PIECE_FLIPPED[4][4];
extern const int SQUARE_PIECE[4][4];
extern const int S_PIECE[4][4];
extern const int S_PIECE_FLIPPED[4][4];
extern const int T_PIECE[4][4];

#endif /* INC_PIECE_H_ */
