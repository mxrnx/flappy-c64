#include <c64.h>
#include <conio.h>

#include "common.h"
#include "sprites.h"
#include "score.h"

#define BIRD_HEIGHT 11

#define FLY_SPEED 9
#define FLY_DURATION 10
#define FLY_AGAIN_THRESHOLD 8

#define FALL_SPEED 3
#define PIPE_SPEED 6

#define PIPE_MAX_X 401

// graphics.s
extern void clearScreen(void);
extern void setTextColor(const unsigned char color);
extern void setFrameColor(const unsigned char color);
extern void setBackgroundColor(const unsigned char color);

// frames.s
extern void setupRasterInterrupt(void);
volatile unsigned char frames = 0;

// input.s
extern unsigned char isSpaceOrShootDown(void);

void updatePipe(unsigned int* pipe_x) {
	unsigned int pipe_x_start = *pipe_x;
	*pipe_x -= PIPE_SPEED;

	if (*pipe_x > PIPE_MAX_X) { // Put a limit on overflow
		*pipe_x = PIPE_MAX_X;
	} else if (pipe_x_start >= POS_BIRD && *pipe_x <= POS_BIRD) {
		increaseAndDrawScore();
	}
}

void waitForKey(unsigned char key) {
	asm("lda #0");
	asm("sta $C6");
	while(cgetc() != key) {}
}

void game(void) {
	unsigned int pipe_a_x = PIPE_MAX_X;
	unsigned int pipe_b_x = PIPE_MAX_X - 200;
	unsigned int bird_y = 100;

	unsigned char flyTimeLeft = 0;

	unsigned char lastUpdate = 0;

	// Prepare game area
	resetScore();
	VIC.spr_coll = 0;
	setFrameColor(COLOR_ORANGE);
	setBackgroundColor(COLOR_LIGHTBLUE);
	updateSprites(bird_y, pipe_a_x, pipe_b_x);

	// Main game loop
	while (1)
	{
		// Game logic //
		if ((unsigned char)(frames - lastUpdate) > 0) {
			bird_y += isSpaceOrShootDown() ? -FLY_SPEED : FALL_SPEED;

			updatePipe(&pipe_a_x);
			updatePipe(&pipe_b_x);

			lastUpdate = frames;
		}

		// Draw //
		updateSprites(bird_y, pipe_a_x, pipe_b_x);

		// If spr_coll's 0 bit is set, there is a collision with the bird
		if (bird_y - BIRD_HEIGHT > POS_BOTTOM || bird_y < POS_TOP || (VIC.spr_coll % 2) == 1)
			break;
	}

	setFrameColor(COLOR_BLACK);
	setBackgroundColor(COLOR_RED);

	cputsxy(9, 12, "Press 'enter' to start");
	waitForKey(13); // 13 = Return
	cputsxy(9, 12, "                      ");
}

void main(void) {
	clearScreen();
	setTextColor(COLOR_WHITE);

	// Start frame timer
	setupRasterInterrupt();

	// Setup sprites
	setupSprites();

	while (1) {
		game();
	}
}
