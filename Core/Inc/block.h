/*
 * block.h
 *
 *  Created on: Jan 13, 2025
 *      Author: nikutsuki
 */

#ifndef INC_BLOCK_H_
#define INC_BLOCK_H_

typedef struct block {
	int x;
	int y;
} block_t;

void render_block(block_t block);
block_t construct_block(int x, int y);

#endif /* INC_BLOCK_H_ */
