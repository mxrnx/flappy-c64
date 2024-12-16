#include <c64.h>
#include <conio.h>

#include "common.h"
#include "sprites.h"

#define BIRD_HEIGHT 11

extern void clearScreen(void);
extern void setFrameColor(const unsigned char color);
extern void setBackgroundColor(const unsigned char color);

const unsigned char KEY_PRESS_MAX = 60;
const unsigned char KEY_PRESS_MIN = 20;
const unsigned int PIPE_MAX_X = 401;

void game(void) {
	unsigned char key;
	unsigned int pipe_a_x = PIPE_MAX_X;
	unsigned int pipe_b_x = PIPE_MAX_X + 200;
	unsigned int bird_y = 100;
	unsigned char frames = 0;
	unsigned char keyPress = 0;

	setFrameColor(COLOR_ORANGE);
	setBackgroundColor(COLOR_LIGHTBLUE);

	// Setup sprites
	setupSprites(bird_y, pipe_a_x, pipe_b_x);

	VIC.spr_coll = 0;

	// Infinite loop
	while (1)
	{
		++frames;

		if (kbhit()) {
			key = cgetc();  // Get the pressed key
			if (key == 'q' || key == 'Q')
				break;  // Exit if 'Q' is pressed

			// Flap
			if (key == ' ' && keyPress < KEY_PRESS_MIN)
				keyPress = KEY_PRESS_MAX;

			// Print the key code
			// cprintf("Key: %c, Code: %u\r\n", key, key);
		}

		if (keyPress > 0) {
			if (frames % 2 == 0)
			{
				--bird_y;
				--keyPress;
			}
		} else if (frames % 3 == 0) {
			++bird_y;
		}

		// TODO: refactor
		if (pipe_a_x > 0)
			--pipe_a_x;
		else
			pipe_a_x = PIPE_MAX_X;

		if (pipe_b_x > 0)
			--pipe_b_x;
		else
			pipe_b_x = PIPE_MAX_X;

		updateSprites(bird_y, pipe_a_x, pipe_b_x);

		if (bird_y - BIRD_HEIGHT > POS_BOTTOM || bird_y < POS_TOP || VIC.spr_coll != 0)
		{
			break;
		}
	}

	setFrameColor(COLOR_BLACK);
	setBackgroundColor(COLOR_RED);

	key = cgetc(); // Wait for user to press a key
}

void main(void) {
	clearScreen();

	while (1) {
		game();
	}
}

// up	left	down	right	space
// 145	157	17	29	32

