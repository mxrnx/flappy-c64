#include <string.h>
#include <c64.h>
#include <peekpoke.h>

#include "common.h"
#include "sprite_data.h"

const unsigned int BIRD_X = 150;

const unsigned char BIRD_SPRITE_NUM = 0;
const unsigned char BOTTOM_PIPE_A_SPRITE_NUM = 1;
const unsigned char TOP_PIPE_A_SPRITE_NUM = 2;
const unsigned char BOTTOM_PIPE_B_SPRITE_NUM = 3;
const unsigned char TOP_PIPE_B_SPRITE_NUM = 4;

void setupSprite(const char sprite, const unsigned char color, const unsigned char* data) {
	unsigned char* spriteMemory = (unsigned char*)(0x3800 + (sprite * 0x80));

	// Copy sprite pattern data to the corresponding memory
	memcpy(spriteMemory, data, 64);

	// Set the sprite pointer for the VIC-II
	POKE(0x400 + 0x3F8 + sprite, (unsigned int)spriteMemory / 64);  // Set sprite pointer (use integer division by 64)

	// Configure the sprite using the __vic2 struct
	VIC.spr_color[sprite] = color;// + sprite;
	VIC.spr_ena |= (1 << sprite);  // Enable specific sprite (bitwise OR)

	// Make larger
	VIC.spr_exp_x |= 1 << sprite;
	VIC.spr_exp_y |= 1 << sprite;
}

void moveSprite(const unsigned char sprite, unsigned int x, unsigned char y) {
	if (x > 255) {
		VIC.spr_hi_x |= (1 << sprite);  // Set the appropriate bit in spr_hi_x for the high byte
	} else {
		VIC.spr_hi_x &= ~(1 << sprite); // Clear the appropriate bit in spr_hi_x if x <= 255
	}

	VIC.spr_pos[sprite].x = x & 0xFF;
	VIC.spr_pos[sprite].y = y;
}

void setupPipe(const unsigned char bottom_sprite_num, const unsigned char top_sprite_num) {
	setupSprite(bottom_sprite_num, COLOR_GREEN, BOTTOM_PIPE_SPRITE_DATA);
	setupSprite(top_sprite_num, COLOR_GREEN, TOP_PIPE_SPRITE_DATA);
}

void updateSprites(const unsigned int bird_y, const unsigned int pipe_a_x, const unsigned int pipe_b_x) {
	moveSprite(BIRD_SPRITE_NUM, BIRD_X, bird_y);
	moveSprite(BOTTOM_PIPE_A_SPRITE_NUM, pipe_a_x, POS_BOTTOM);
	moveSprite(TOP_PIPE_A_SPRITE_NUM, pipe_a_x, POS_TOP);
	moveSprite(BOTTOM_PIPE_B_SPRITE_NUM, pipe_b_x, POS_BOTTOM);
	moveSprite(TOP_PIPE_B_SPRITE_NUM, pipe_b_x, POS_TOP);
}

void setupSprites(const unsigned int bird_y, const unsigned int pipe_a_x, const unsigned int pipe_b_x) {
	setupSprite(BIRD_SPRITE_NUM, COLOR_YELLOW, BIRD_SPRITE_DATA);
	setupPipe(BOTTOM_PIPE_A_SPRITE_NUM, TOP_PIPE_A_SPRITE_NUM);
	setupPipe(BOTTOM_PIPE_B_SPRITE_NUM, TOP_PIPE_B_SPRITE_NUM);
	updateSprites(bird_y, pipe_a_x, pipe_b_x);
}

