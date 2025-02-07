#include <conio.h>
#include <stdlib.h>

char* scoreMemory = (char*)(0x00EE);
int score = 0;

void increaseAndDrawScore() {
	score++;
	itoa(score, scoreMemory, 10);
	cputsxy(0, 0, scoreMemory);
}

void resetScore() {
	cputsxy(0, 0, "       ");
	score = 0;
}
