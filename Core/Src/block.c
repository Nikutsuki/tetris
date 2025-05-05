/*
 * block.c
 *
 *  Created on: Jan 13, 2025
 *      Author: nikutsuki
 */


#include "block.h"
#include "ssd1306.h"

void render_block(block_t block)
{
	ssd1306_DrawRectangle(block.x, block.y, block.x + 6, block.y + 6, White);
}

block_t construct_block(int x, int y)
{
	block_t block;
	block.x = x;
	block.y = y;
	return block;
}