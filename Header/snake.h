#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "common.h"

// point struct definition
typedef struct {
	unsigned short x, y;	// x and y coordinates of the point
	char direction;			// the direction where the point is headed (relevant if the point is on the snake)
} Point;

// Snake struct definition
typedef struct {
	unsigned short bend_no, body_no, length, lives;		// length and lives of the snake
	unsigned int score;		// score
} Snake;

#define SNAKE_DEFAULT_LIVES 3
#define DEFAULT_BODY_LENGTH 5		// the initial length of the snake is 5, and it can reach a maximum length of 29
#define MAX_BODY_LENGTH 29
#define SNAKE_BOARD_WIDTH 60
#define SNAKE_BOARD_HEIGHT 20
const Point SPAWN_POINT = (Point){25, 15, RIGHT_KEY};

Point head, bend[500], food, body[MAX_BODY_LENGTH+1];
Snake snake = (Snake){0, 0, DEFAULT_BODY_LENGTH + 1, SNAKE_DEFAULT_LIVES, 0};
unsigned short game_on_flag_2 = TRUE;

void OpeningScreen2();
void UpdateFrame2();
void PrintBoard2();
void GenerateFood();
void SnakeMove();
void SnakeBend();
void LoseaLife();
unsigned short RecordScore2();
void PlaySnake();


// welcome screen of the Snake game where the instructions of the game is presented to the player
void OpeningScreen2() {
	system("cls");
	printf( "Welcome to the mini Snake game!"
			"\n\n\nGame instructions:"
			"\n\n-> Use arrow keys to move the snake."
			"\n\n-> You will be provided foods at the several coordinates of the screen which you have to eat. Everytime you eat a food the length of the snake will be increased by 1 element and thus the score."
			"\n\n-> Here you are provided with %d lives. Your lives will decrease as you hit the wall or snake's body."
			"\n\n-> You can pause the game in its middle by pressing the P key. To continue the paused game press P once again."
			"\n\n-> If you want to exit the game at any point press Esc (you will lose all progress)."
			"\n\n\nPress any key to play the game...", SNAKE_DEFAULT_LIVES);
	if(getch() == ESCAPE_KEY)
		exit(0);
	system("cls");
}

// updates the frame of the Snake game
void UpdateFrame2() {
	do {
		GenerateFood();
		snake.body_no = 0;
		Sleep(35);				// FPS: 1000 ms / 35 ms = 28.571428
		SnakeMove();
		LoseaLife();
		if(kbhit()) {
			char key = getch();
			switch(key) {
				case LEFT_KEY:
				case RIGHT_KEY:
					switch(head.direction) {
						case UP_KEY:
						case DOWN_KEY:
							bend[++snake.bend_no] = head;
							head.direction = key;
							switch(key) {
								case LEFT_KEY:
									head.x--;
									break;
								case RIGHT_KEY:
									head.x++;
									break;
								case UP_KEY:
									head.y--;
									break;
								case DOWN_KEY:
									head.y++;
							}
					}
					break;
				case UP_KEY:
				case DOWN_KEY:
					switch(head.direction) {
						case LEFT_KEY:
						case RIGHT_KEY:
							bend[++snake.bend_no] = head;
							head.direction = key;
							switch(key) {
								case LEFT_KEY:
									head.x--;
									break;
								case RIGHT_KEY:
									head.x++;
									break;
								case UP_KEY:
									head.y--;
									break;
								case DOWN_KEY:
									head.y++;
							}
					}
					break;
				case PAUSE_KEY:		// pause the game
					do {
						key = getch();
						if(key == ESCAPE_KEY) {
							MoveCursorToXY(0, 5 + SNAKE_BOARD_HEIGHT);
							exit(0);
						}
					} while(key != PAUSE_KEY);
					break;
				case ESCAPE_KEY:	// exit the game
					MoveCursorToXY(0, 5 + SNAKE_BOARD_HEIGHT);
					exit(0);
			}
		}
	} while(game_on_flag_2);
	system("cls");
	printf( "Game over!"			// game over
			"\nBetter luck next time."
			"\n(Press C to continue...)");
	char key;
	do {
		key = getch();
		if(key == ESCAPE_KEY)
			exit(0);
	} while(key!='c' && key!='C');
	system("cls");
}

// prints the board of the Snake game, including player scores and lives
void PrintBoard2() {
	MoveCursorToXY(20, 4);
	printf("SCORE: %u", snake.score);
	MoveCursorToXY(50, 4);
	printf("LIVES: %hu", snake.lives);
	unsigned short i;
	for(i=10;i<11+SNAKE_BOARD_WIDTH;i++) {
		MoveCursorToXY(i, 5);
		printf("_");
		MoveCursorToXY(i, 5 + SNAKE_BOARD_HEIGHT);
		printf("_");
	}
	for(i=6;i<6+SNAKE_BOARD_HEIGHT;i++) {
		MoveCursorToXY(10, i);
		printf("|");
		MoveCursorToXY(10 + SNAKE_BOARD_WIDTH, i);
		printf("|");
	}
}

// generates food on the board for the snake to eat
void GenerateFood() {
	unsigned short snake_intersect_flag = TRUE;
	if(head.x == food.x && head.y == food.y) {
		if(snake.length <= MAX_BODY_LENGTH)
			snake.length++;
		MoveCursorToXY(27, 4);
		printf("%u", ++snake.score);
		MoveCursorToXY(food.x, food.y);				// erase the eaten food from board
		printf(" ");
		while(snake_intersect_flag) {
			food.x = rand()%(SNAKE_BOARD_WIDTH - 1) + 11;
			food.y = rand()%(SNAKE_BOARD_HEIGHT - 1) + 6;
			unsigned short i;
			for(i=0;i<snake.length;i++)
				if(food.x == body[i].x && food.y == body[i].y)
					break;
				else if(i == snake.length - 1)
					snake_intersect_flag = FALSE;
		}
		MoveCursorToXY(food.x, food.y);				// shows the food on board with "F"
		printf("F");
	}
	else if(food.x == 0 && food.y == 0) {			// generates food for the first time
		while(snake_intersect_flag) {
			food.x = rand()%(SNAKE_BOARD_WIDTH - 1) + 11;
			food.y = rand()%(SNAKE_BOARD_HEIGHT - 1) + 6;
			unsigned short i;
			for(i=0;i<snake.length;i++)
				if(food.x == body[i].x && food.y == body[i].y)
					break;
				else if(i == snake.length - 1)
					snake_intersect_flag = FALSE;
		}
		MoveCursorToXY(food.x, food.y);
		printf("F");
	}
}

// moves the snake by one unit in the direction the snake is facing
void SnakeMove() {
	unsigned short i;
	switch(head.direction) {
		case LEFT_KEY:
			for(i=0; i <= bend[snake.bend_no].x - head.x && snake.body_no < snake.length; i++, snake.body_no++) {
				body[snake.body_no].x = head.x + i;
				body[snake.body_no].y = head.y;
				MoveCursorToXY(body[snake.body_no].x, body[snake.body_no].y);
				printf(snake.body_no == 0 ? "<" : "*");
			}
			SnakeBend();
			if(!kbhit())
				head.x--;
			break;
		case RIGHT_KEY:
			for(i=0; i <= head.x - bend[snake.bend_no].x && snake.body_no < snake.length; i++, snake.body_no++) {
				body[snake.body_no].x = head.x - i;
				body[snake.body_no].y = head.y;
				MoveCursorToXY(body[snake.body_no].x, body[snake.body_no].y);
				printf(snake.body_no == 0 ? ">" : "*");
			}
			SnakeBend();
			if(!kbhit())
				head.x++;
			break;
		case UP_KEY:
			for(i=0; i <= bend[snake.bend_no].y - head.y && snake.body_no < snake.length; i++, snake.body_no++) {
				body[snake.body_no].x = head.x;
				body[snake.body_no].y = head.y + i;
				MoveCursorToXY(body[snake.body_no].x, body[snake.body_no].y);
				printf(snake.body_no == 0 ? "^" : "*");
			}
			SnakeBend();
			if(!kbhit())
				head.y--;
			break;
		case DOWN_KEY:
			for(i=0; i <= head.y - bend[snake.bend_no].y && snake.body_no < snake.length; i++, snake.body_no++) {
				body[snake.body_no].x = head.x;
				body[snake.body_no].y = head.y - i;
				MoveCursorToXY(body[snake.body_no].x, body[snake.body_no].y);
				printf(snake.body_no == 0 ? "v" : "*");
			}
			SnakeBend();
			if(!kbhit())
				head.y++;
	}
}

// updates the body of the snake in case the snake bends
void SnakeBend() {
	MoveCursorToXY(body[snake.length-1].x, body[snake.length-1].y);
	printf(" ");
	short i, j, diff;
	for(i=snake.bend_no; i >= 0 && snake.body_no < snake.length; i--)
		if(bend[i].x == bend[i-1].x) {
			diff = bend[i].y - bend[i-1].y;
			for(j=1; j <= abs(diff) && snake.body_no < snake.length; j++, snake.body_no++) {
				body[snake.body_no].x = bend[i].x;
				body[snake.body_no].y = diff<0 ? bend[i].y + j : bend[i].y - j;
				MoveCursorToXY(body[snake.body_no].x, body[snake.body_no].y);
				printf("*");
			}
			MoveCursorToXY(body[snake.length-1].x, body[snake.length-1].y);
			printf(" ");
		}
		else if(bend[i].y == bend[i-1].y) {
			diff = bend[i].x - bend[i-1].x;
			for(j=1; j <= abs(diff) && snake.body_no < snake.length; j++, snake.body_no++) {
				body[snake.body_no].x = diff<0 ? bend[i].x + j : bend[i].x - j;
				body[snake.body_no].y = bend[i].y;
				MoveCursorToXY(body[snake.body_no].x, body[snake.body_no].y);
				printf("*");
			}
			MoveCursorToXY(body[snake.length-1].x, body[snake.length-1].y);
			printf(" ");
		}
}

// erases a life from the snake when the snake hits its body or the board
void LoseaLife() {
	unsigned short i, touched_body_flag = FALSE;
	for(i=4;i<snake.length;i++)				// starts from 4 because the snake has to be of at least length 4 to touch its body
		if(body[0].x == body[i].x && body[0].y == body[i].y) {
			touched_body_flag = TRUE;		// check if the head hits the body
			break;
		}
	if(head.x <= 10 || head.x >= 10 + SNAKE_BOARD_WIDTH || head.y <= 5 || head.y >= 5 + SNAKE_BOARD_HEIGHT || touched_body_flag) {
		for(i=0;i<snake.length-1;i++) {
			MoveCursorToXY(body[i].x, body[i].y);
			printf("X");
			Sleep(80);
			printf("\b ");
		}
		MoveCursorToXY(57, 4);
		printf("%hu", --snake.lives);
		if(!snake.lives)
			game_on_flag_2 = FALSE;
		head = SPAWN_POINT;
		snake.bend_no = 0;
		for(i=0;i<snake.length;i++)
			body[i].x = body[i].y = 0;
	}
}

// records the scores of the Snake game in "Records"
unsigned short RecordScore2() {
	time_t mytime = time(NULL);
	char player_name[200];
	printf("Enter your name:\n");
	gets(player_name);
	FILE *info = fopen("./Records/snake_record.txt", "a+");
	fprintf(info, "Player Name: %s\n", player_name);
	fprintf(info, "Played Date: %s", ctime(&mytime));
	fprintf(info, "Score: %u\n", snake.score);
	fprintf(info, "__________________________________________________\n");
	fclose(info);
	system("cls");
	printf( "Press Y to see past records."
			"\nPress R to play again."
			"\nPress M to return to the menu."
			"\nPress any other key to exit.");
	char key = getch();
	switch(key) {
		case 'Y':
		case 'y':
			system("cls");
			info = fopen("./Records/snake_record.txt", "r");
			char ch;
			while((ch = fgetc(info)) != EOF)
				printf("%c", ch);
			fclose(info);
			break;
		case 'R':
		case 'r':
			game_on_flag_2 = TRUE;
			snake = (Snake){0, 0, DEFAULT_BODY_LENGTH + 1, SNAKE_DEFAULT_LIVES, 0};
			food.x = food.y = 0;
			system("cls");
			return TRUE;
		case 'M':
		case 'm':
			return FALSE;
	}
	exit(0);
}

// the main function to play Snake
void PlaySnake() {
	unsigned short play_again_flag = FALSE;
	do {
		OpeningScreen2();
		LoadingScreen();
		PrintBoard2();
		bend[0] = head = SPAWN_POINT;
		UpdateFrame2();
		play_again_flag = RecordScore2();
	} while(play_again_flag);
}

#endif
