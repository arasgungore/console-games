#include "Header/pong.h"
#include "Header/snake.h"
#include "Header/spaceInvaders.h"
#include "Header/tetris.h"

typedef enum {
	PONG='1', SNAKE, SPACE_INVADERS, TETRIS
} ArcadeGames;

int main() {
	srand((unsigned int)time(NULL));
	while(TRUE) {
		printf( "Press 1 to play Pong."
				"\nPress 2 to play Snake."
				"\nPress 3 to play Space Invaders."
				"\nPress 4 to play Tetris."
				"\nPress Esc to exit.");
		char choose_game = getch();
		switch(choose_game) {
			case PONG:
				PlayPong();
				break;
			case SNAKE:
				PlaySnake();
				break;
			case SPACE_INVADERS:
				PlaySpaceInvaders();
				break;
			case TETRIS:
				PlayTetris();
				break;
			case ESCAPE_KEY:
				exit(0);
		}
		system("cls");
	}
}
