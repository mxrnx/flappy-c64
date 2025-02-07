#include <c64.h>
#include <conio.h>
#include <stdlib.h>
#include <peekpoke.h>

#include "common.h"
#include "sprites.h"

#define BIRD_HEIGHT 11

#define FLY_SPEED 6
#define FLY_DURATION 10
#define FLY_AGAIN_THRESHOLD 4

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

char* scoreMemory = (char*)(0x00EE); // is this even safe?
int score;

void updatePipe(unsigned int* pipe_x) {
	unsigned int pipe_x_start = *pipe_x;
	*pipe_x -= PIPE_SPEED;

	if (*pipe_x > PIPE_MAX_X) { // Put a limit on overflow
		*pipe_x = PIPE_MAX_X;
	} else if (pipe_x_start >= 150 && *pipe_x <= 150) { // TODO constant
		score++;
		itoa(score, scoreMemory, 10);
		cputsxy(0, 0, scoreMemory);
	}
}

void game(void) {
	unsigned char key;

	unsigned int pipe_a_x = PIPE_MAX_X;
	unsigned int pipe_b_x = PIPE_MAX_X - 200;
	unsigned int bird_y = 100;

	unsigned char flyTimeLeft = 0;

	unsigned char lastUpdate = 0;

	cputsxy(0, 0, "       ");
	score = 0;

	// Start frame timer
	setupRasterInterrupt();

	// Prepare game area
	setTextColor(COLOR_WHITE);
	setFrameColor(COLOR_ORANGE);
	setBackgroundColor(COLOR_LIGHTBLUE);
	updateSprites(bird_y, pipe_a_x, pipe_b_x);

	// Main game loop
	while (1)
	{
		// Handle inputs //
		/*if (kbhit() && cgetc() == ' ' && flyTimeLeft < FLY_AGAIN_THRESHOLD)
			flyTimeLeft = FLY_DURATION;*/

		if ((PEEK(56321) & 16) == 0  && flyTimeLeft < FLY_AGAIN_THRESHOLD)
			flyTimeLeft = FLY_DURATION;

		// Update game logic //
		if ((unsigned char)(frames - lastUpdate) > 0) {
			if (flyTimeLeft > 0) {
				bird_y -= FLY_SPEED;
				--flyTimeLeft;
			} else {
				bird_y += FALL_SPEED;
			}

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

	key = cgetc(); // Wait for user to press a key
}

void main(void) {
	clearScreen();

	// Setup sprites
	setupSprites();

	while (1) {
		game();
	}
}
