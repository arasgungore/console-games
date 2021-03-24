#ifndef _TETRIS_H_
#define _TETRIS_H_

#include "common.h"

// shape (tetromino) struct definition
typedef struct {
	char **shape_matrix;		// stores the shape of the tetromino in a char matrix
	unsigned short width;		// width of the tetromino
	short x, y;					// x and y coordinates of the location of tetromino
} Shape;

#define TETRIS_BOARD_WIDTH 11
#define TETRIS_BOARD_HEIGHT 20

// Tetris struct definition
typedef struct {
	char board[TETRIS_BOARD_HEIGHT][TETRIS_BOARD_WIDTH];	// game board
	unsigned int score, timer;								// score and timer
} Tetris;

#define ROTATE_KEY UP_KEY
#define DEFAULT_TIMER 500000		// 500000 microseconds = half a second

// 7 tetrominos that are going to be used throughout the game
const Shape Tetrominos[7] = {
	{(char *[]){(char []){0,0,0,0},
				(char []){1,1,1,1},
				(char []){0,0,0,0},
				(char []){0,0,0,0}}, 4, 0, 0},		// I tetromino
	{(char *[]){(char []){1,1},
				(char []){1,1}}, 2, 0, 0},			// O tetromino
	{(char *[]){(char []){0,1,0},
				(char []){1,1,1},
				(char []){0,0,0}}, 3, 0, 0},		// T tetromino
	{(char *[]){(char []){0,1,1},
				(char []){1,1,0},
				(char []){0,0,0}}, 3, 0, 0},		// S tetromino
	{(char *[]){(char []){1,1,0},
				(char []){0,1,1},
				(char []){0,0,0}}, 3, 0, 0},		// Z tetromino
	{(char *[]){(char []){0,0,1},
				(char []){1,1,1},
				(char []){0,0,0}}, 3, 0, 0},		// J tetromino
	{(char *[]){(char []){1,0,0},
				(char []){1,1,1},
				(char []){0,0,0}}, 3, 0, 0}			// L tetromino
};

Shape *current;
Tetris tetris = (Tetris){{{0}}, 0, DEFAULT_TIMER};
unsigned short game_on_flag_4 = TRUE;

void OpeningScreen4();
void UpdateFrame4();
void PrintBoard4();
void PrintShapeToConsole();
void EraseShapeFromConsole();
void WriteShapeToBoard();
void DeleteShapeFromBoard();
void GetNewShape();
Shape* CopyShape(const Shape shape);
void DeleteShape(Shape *shape);
unsigned short CheckPosition(Shape *shape);
void CheckRows();
void RotateShape(Shape *shape);
void ControlCurrentShape(char key);
unsigned short RecordScore4();
void PlayTetris();


// welcome screen of the Tetris game where the instructions of the game is presented to the player
void OpeningScreen4() {
	system("cls");
	printf( "Welcome to the mini Tetris game!"
			"\n\n\nGame instructions:"
			"\n\n-> Use left and right arrow keys to move blocks across the screen, down arrow key to bring them down faster, and the up arrow key to rotate them."
			"\n\n-> Your objective is to get all the blocks to fill all the empty space in a row at the bottom of the screen. Thus, the filled row will vanish and you get awarded 100 points."
			"\n\n-> Your game is over if a block reaches the top of the screen."
			"\n\n-> You can pause the game in its middle by pressing the P key. To continue the paused game press P once again."
			"\n\n-> If you want to exit the game at any point press Esc (you will lose all progress)."
			"\n\n\nPress any key to play the game...");
	if(getch() == ESCAPE_KEY)
		exit(0);
	system("cls");
}

// updates the frame of the Tetris game
void UpdateFrame4() {
	struct timeval before, after;
	gettimeofday(&before, NULL);
	do {
		if(kbhit()) {
			char key = getch();					// FPS: 10^6 us / tetris.timer us = 1 s / 0.5 s = 2
			ControlCurrentShape(key);			// the FPS will gradually increase because every time a row is filled, tetris.timer will decrease by a millisecond
		}
		gettimeofday(&after, NULL);
		if((unsigned int)(after.tv_sec*1000000 + after.tv_usec - before.tv_sec*1000000 - before.tv_usec) > tetris.timer) {
			before = after;
			ControlCurrentShape(DOWN_KEY);		// in every once in "timer", the tetromino will fall by a unit on the board downwards
		}										// as the game progresses, pieces will start to fall faster and faster
	} while(game_on_flag_4);
	system("cls");
	printf( "Game over!"		// game over
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

// prints the board of the Tetris game, including player's score
void PrintBoard4() {
	MoveCursorToXY(30, 2);
	printf("SCORE: %u", tetris.score);
	MoveCursorToXY(30, 3);
	unsigned short i;
	for(i=0;i<2*TETRIS_BOARD_WIDTH+2;i++)
		printf("*");
	for(i=0;i<TETRIS_BOARD_HEIGHT;i++) {
		MoveCursorToXY(30, 4 + i);
		printf("*");
		MoveCursorToXY(31 + 2*TETRIS_BOARD_WIDTH, 4 + i);
		printf("*");
	}
	MoveCursorToXY(30, 4 + TETRIS_BOARD_HEIGHT);
	for(i=0;i<2*TETRIS_BOARD_WIDTH+2;i++)	
		printf("*");
}

// prints the shapes onto the terminal
void PrintShapeToConsole() {
	unsigned short i, j;
	for(i=0;i<TETRIS_BOARD_HEIGHT;i++)
		for(j=0;j<TETRIS_BOARD_WIDTH;j++)
			if(tetris.board[i][j]) {
				MoveCursorToXY(31 + 2*j, 4 + i);
				printf("O");
			}
}

// erases the shape seen on the terminal
void EraseShapeFromConsole() {
	unsigned short i, j;
	for(i=0;i<current->width;i++)
		for(j=0;j<current->width;j++)
			if(current->shape_matrix[i][j]) {
				MoveCursorToXY(31 + 2*(j + current->x), 4 + i + current->y);
				printf(" ");
			}
}

// stores the shape in the board with ones
void WriteShapeToBoard() {
	unsigned short i, j;
	for(i=0;i<current->width;i++)
		for(j=0;j<current->width;j++)
			if(current->shape_matrix[i][j])
				tetris.board[current->y + i][current->x + j] = 1;
}

// deletes the stored shape with zeros
void DeleteShapeFromBoard() {
	unsigned short i,j;
	for(i=0;i<current->width;i++)
		for(j=0;j<current->width;j++)
			if(current->shape_matrix[i][j])
				tetris.board[current->y + i][current->x + j] = 0;
}

// selects a random tetromino
void GetNewShape() {
	current = CopyShape(Tetrominos[rand()%7]);
	current->x = rand()%(TETRIS_BOARD_WIDTH - current->width + 1);
	if(!CheckPosition(current))
		game_on_flag_4 = FALSE;
}

// gets a separate copy of the given shape
Shape* CopyShape(const Shape shape) {
	Shape *copy = (Shape*) malloc(sizeof(Shape));
	copy->width = shape.width;
	copy->y = shape.y;
	copy->x = shape.x;
	copy->shape_matrix = (char**) malloc(copy->width * sizeof(char*));
	unsigned short i, j;
	for(i=0;i<copy->width;i++) {
		copy->shape_matrix[i] = (char*) malloc(copy->width * sizeof(char));
		for(j=0;j<copy->width;j++)
			copy->shape_matrix[i][j] = shape.shape_matrix[i][j];
	}
	return copy;
}

// deletes the given shape and deallocates memory
void DeleteShape(Shape *shape) {
	unsigned short i;
	for(i=0;i<shape->width;i++)
		free(shape->shape_matrix[i]);
	free(shape->shape_matrix);
	free(shape);
}

// checks the position of the shape
unsigned short CheckPosition(Shape *shape) {
	unsigned short i, j;
	for(i=0;i<shape->width;i++)
		for(j=0;j<shape->width;j++)
			if(shape->shape_matrix[i][j]) {
				if(shape->x + j < 0 || shape->x + j >= TETRIS_BOARD_WIDTH || shape->y + i >= TETRIS_BOARD_HEIGHT)	// if out of board boundaries
					return FALSE;
				else if(tetris.board[shape->y + i][shape->x + j])		// if another piece occupies the given position
					return FALSE;
			}
	return TRUE;
}

// checks if we have hit a full row or not
// if we have, adds 100 to the score for each filled row
void CheckRows() {
	unsigned short i, j, counter = 0;
	for(i=0;i<TETRIS_BOARD_HEIGHT;i++) {
		unsigned short sum = 0;
		for(j=0;j<TETRIS_BOARD_WIDTH;j++)
			sum += tetris.board[i][j];
		if(sum == TETRIS_BOARD_WIDTH) {
			counter++;
			for(j=0;j<TETRIS_BOARD_WIDTH;j++) {
				MoveCursorToXY(31 + 2*j, 4 + i);
				printf(" ");
			}
			unsigned short k;
			for(k=i;k>=1;k--)
				for(j=0;j<TETRIS_BOARD_WIDTH;j++) {
					if(!tetris.board[k-1][j] && tetris.board[k][j]) {
						MoveCursorToXY(31 + 2*j, 4 + k);
						printf(" ");
					}
					tetris.board[k][j] = tetris.board[k-1][j];
				}
			for(j=0;j<TETRIS_BOARD_WIDTH;j++) {
				tetris.board[k][j] = 0;
				MoveCursorToXY(31 + 2*j, 4 + k);
				printf(" ");
			}
		}
	}
	tetris.timer -= 1000;
	tetris.score += 100*counter;
	MoveCursorToXY(37, 2);
	printf("%u", tetris.score);
}

// rotates the given tetromino clockwise
void RotateShape(Shape *shape) {
	Shape *temp = CopyShape(*shape);
	unsigned short i, j;
	for(i=0;i<shape->width;i++)
		for(j=0;j<shape->width;j++)
			shape->shape_matrix[i][j] = temp->shape_matrix[shape->width - j - 1][i];
	DeleteShape(temp);
}

// manipulates the tetromino according to which key is pressed
void ControlCurrentShape(char key) {
	Shape *temp = CopyShape(*current);
	switch(key) {
		case LEFT_KEY:				// if hit left arrow key, move the piece to the left
			EraseShapeFromConsole();
			temp->x--;
			if(CheckPosition(temp))
				current->x--;
			break;
		case RIGHT_KEY:				// if hit right arrow key, move the piece to the right
			EraseShapeFromConsole();
			temp->x++;
			if(CheckPosition(temp))
				current->x++;
			break;
		case DOWN_KEY:				// if hit down arrow key, move the piece downwards
			EraseShapeFromConsole();
			temp->y++;
			if(CheckPosition(temp))
				current->y++;
			else {
				WriteShapeToBoard();
				CheckRows();		// after placing the shape, check if the any rows are filled
				GetNewShape();
			}
			break;
		case ROTATE_KEY:			// if hit up arrow key, rotate the shape
			EraseShapeFromConsole();
			RotateShape(temp);
			if(CheckPosition(temp))
				RotateShape(current);
			break;
		case PAUSE_KEY:				// pause the game
			do {
				key = getch();
				if(key == ESCAPE_KEY) {
					MoveCursorToXY(0, 4 + TETRIS_BOARD_HEIGHT);
					exit(0);
				}
			} while(key != PAUSE_KEY);
			break;
		case ESCAPE_KEY:			// exit the game
			MoveCursorToXY(0, 4 + TETRIS_BOARD_HEIGHT);
			exit(0);
	}
	DeleteShape(temp);
	WriteShapeToBoard();
	PrintShapeToConsole();
	DeleteShapeFromBoard();
}

// records the scores of the Tetris game in "Records"
unsigned short RecordScore4() {
	time_t mytime = time(NULL);
	char player_name[200];
	printf("Enter your name:\n");
	gets(player_name);
	FILE *info = fopen("./Records/tetris_record.txt", "a+");
	fprintf(info, "Player Name: %s\n", player_name);
	fprintf(info, "Played Date: %s", ctime(&mytime));
	fprintf(info, "Score: %u\n", tetris.score);
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
			info = fopen("./Records/tetris_record.txt","r");
			char ch;
			while((ch = fgetc(info)) != EOF)
				printf("%c", ch);
			fclose(info);
			break;
		case 'R':
		case 'r':
			game_on_flag_4 = TRUE;
			tetris = (Tetris){{{0}}, 0, DEFAULT_TIMER};
			system("cls");
			return TRUE;
		case 'M':
		case 'm':
			return FALSE;
	}
	exit(0);
}

// the main function to play Tetris
void PlayTetris() {
	unsigned short play_again_flag = FALSE;
	do {
		OpeningScreen4();
		LoadingScreen();
		GetNewShape();
		PrintBoard4();
		UpdateFrame4();
		play_again_flag = RecordScore4();
	} while(play_again_flag);
}

#endif
