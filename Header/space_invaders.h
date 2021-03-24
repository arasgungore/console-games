#ifndef _SPACE_INVADERS_H_
#define _SPACE_INVADERS_H_

#include "common.h"

#define SI_BOARD_WIDTH 33
#define SI_BOARD_HEIGHT 19

// Space Invaders struct definition
typedef struct {
	char board[SI_BOARD_HEIGHT][SI_BOARD_WIDTH+1];		// game board
	unsigned short ship_x, lives;						// x coordinate of the ship (aka hero) and how many lives it has
	unsigned int score;									// score
} SpaceInvaders;

#define SHOOT_KEY UP_KEY
#define SI_DEFAULT_LIVES 3
#define DEFAULT_SHIP_POSITION 16
#define ENEMY_COUNT 42				// number of enemies the hero has to kill in order to win the game
#define MAX_BULLET_COUNT 5			// the board cannot have more than 5 bullets on one frame at once

#define HERO 'H'					// ASCII representations of the elements in the game
#define ENEMY '@'
#define BOMB 'U'
#define BULLET '^'
#define IMPACT 'X'
#define EMPTY ' '
const char SI_SPAWN_BOARD[SI_BOARD_HEIGHT][SI_BOARD_WIDTH+1] = {		// starting position of the board
		"*********************************",
		"*  @ @ @ @ @ @ @ @ @ @ @ @ @ @  *",
		"*     @ @ @ @ @ @ @ @ @ @ @     *",
		"*        @ @ @ @ @ @ @ @        *",
		"*           @ @ @ @ @           *",
		"*             @ @ @             *",
		"*               @               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*                               *",
		"*               H               *",
		"*********************************"};

SpaceInvaders SI = (SpaceInvaders){{{0}}, DEFAULT_SHIP_POSITION, SI_DEFAULT_LIVES, 0};
unsigned short game_on_flag_3 = TRUE;

void OpeningScreen3();
void UpdateFrame3();
void PrintBoard3();
void CopySpawnBoard();
unsigned short IncreaseSIScore();
unsigned short BulletShoot();
void BulletMove();
void BulletRemove();
void BulletImpact();
void RemoveImpact();
unsigned short BottomEnemyCheck(unsigned short bomb_x, unsigned short y);
void BombDrop(unsigned short bomb_x);
void BombMove();
void BombRemove();
unsigned short BombImpact();
unsigned short EnemyTurn();
void EnemyMoveLeftRight();
void EnemyMoveDown();
unsigned short RecordScore3();
void PlaySpaceInvaders();


// welcome screen of the Space Invaders game where the instructions of the game is presented to the player
void OpeningScreen3() {
	system("cls");
	printf( "Welcome to the mini Space Invaders game!"
			"\n\n\nGame instructions:"
			"\n\n-> Use left and right arrow keys to move the hero, up arrow key to shoot bullets. There can be a maximum of %d bullets on the screen at once."
			"\n\n-> As the hero, your mission is to stop aliens from invading the Earth. For every invader you kill, your score will increase by 10 points. Kill all invaders to save the Earth and win the game."
			"\n\n-> Here you are provided with %d lives. Your lives will decrease as invaders manage to bomb and kill you."
			"\n\n-> You can pause the game in its middle by pressing the P key. To continue the paused game press P once again."
			"\n\n-> If you want to exit the game at any point press Esc (you will lose all progress)."
			"\n\n\nPress any key to play the game...", MAX_BULLET_COUNT, SI_DEFAULT_LIVES);
	if(getch() == ESCAPE_KEY)
		exit(0);
	system("cls");
}

// updates the frame of the Space Invaders game
void UpdateFrame3() {
	unsigned short i;
	for(i=0;game_on_flag_3;i++) {
		PrintBoard3();
		Sleep(25);				// FPS: 1000 ms / 25 ms = 50
		if(kbhit()) {
			char key = getch();
			switch(key) {
				case RIGHT_KEY:			// if hit right arrow key, move the hero to the right by one unit
					if(SI.ship_x<SI_BOARD_WIDTH-2) {
						SI.board[SI_BOARD_HEIGHT-2][SI.ship_x] = EMPTY;
						SI.board[SI_BOARD_HEIGHT-2][++SI.ship_x] = HERO;
					}
					break;
				case LEFT_KEY:			// if hit left arrow key, move the hero to the left by one unit
					if(SI.ship_x>1) {
						SI.board[SI_BOARD_HEIGHT-2][SI.ship_x] = EMPTY;
						SI.board[SI_BOARD_HEIGHT-2][--SI.ship_x] = HERO;
					}
					break;
				case SHOOT_KEY:			// if hit the up arrow key, shoot a bullet
					if(BulletShoot())
						SI.board[SI_BOARD_HEIGHT-3][SI.ship_x] = BULLET;
					break;
				case PAUSE_KEY:			// pause the game
					do {
						key = getch();
						if(key == ESCAPE_KEY)
							exit(0);
					} while(key != PAUSE_KEY);
					break;
				case ESCAPE_KEY:		// exit the game
					exit(0);
			}
		}
		RemoveImpact();
		BombRemove();
		BulletRemove();
		if(BombImpact()) {
			PrintBoard3();
			if(!SI.lives) {
				game_on_flag_3 = FALSE;
				MoveCursorToXY(36, 5 + SI_BOARD_HEIGHT);		// game over
				printf("Invaders killed you.");
				MoveCursorToXY(36, 6 + SI_BOARD_HEIGHT);
				printf("Mission failed. We'll get 'em next time...");
				MoveCursorToXY(36, 7 + SI_BOARD_HEIGHT);
				printf("(Press C to continue...)");
				char key;
				do {
					key = getch();
					if(key == ESCAPE_KEY)
						exit(0);
				} while(key!='c' && key!='C');
				system("cls");
				return;
			}
			Sleep(400);
			SI.board[SI_BOARD_HEIGHT-2][SI.ship_x] = HERO;
		}
		BulletImpact();
		if(!(i%4)) {
			BombMove();					// the bullets and bombs will move in every 4 frames
			BulletMove();
		}
		if(!(i%3))
			BombDrop(rand()%17 + 4);	// bombs will drop in every 3 frames
		if(!(i%7))
			EnemyMoveLeftRight();		// the enemies will move in every 7 frames
	}
}

// prints the board of the Space Invaders game, including player's score and lives
void PrintBoard3() {
	MoveCursorToXY(36, 3);
	printf("SCORE: %u     LIVES: %hu", SI.score, SI.lives);
	unsigned short y;
	for(y=0;y<SI_BOARD_HEIGHT;y++) {
		MoveCursorToXY(36, 4 + y);
		printf("%s", SI.board[y]);
	}
}

// copies the starting position of the board onto the current board
void CopySpawnBoard() {
	unsigned short x, y;
	for(y=0;y<SI_BOARD_HEIGHT;y++)
		for(x=0;x<SI_BOARD_WIDTH+1;x++)
			SI.board[y][x] = SI_SPAWN_BOARD[y][x];
}

// increases the score by 10 points and ends the game if all enemies are dead
unsigned short IncreaseSIScore() {
	SI.score += 10;
	if(SI.score == 10*ENEMY_COUNT) {
		game_on_flag_3 = FALSE;
		PrintBoard3();
		MoveCursorToXY(36, 5 + SI_BOARD_HEIGHT);
		printf("You saved the Earth from invaders.");
		MoveCursorToXY(36, 6 + SI_BOARD_HEIGHT);
		printf("CONGRATULATION! A WINRAR IS YOU!!!");
		MoveCursorToXY(36, 7 + SI_BOARD_HEIGHT);
		printf("(Press C to continue...)");
		char key;
		do {
			key = getch();
			if(key == ESCAPE_KEY)
				exit(0);
		} while(key!='c' && key!='C');
		system("cls");
		return TRUE;
	}
	return FALSE;
}

// checks the number of bullets in the screen doesn't exceed "MAX_BULLET_COUNT"
unsigned short BulletShoot() {
	unsigned short x, y, bullet_count = 0;
	for(y=SI_BOARD_HEIGHT-3;y>0;y--)
		for(x=1;x<SI_BOARD_WIDTH-1;x++)
			if(SI.board[y][x] == BULLET)
				if(++bullet_count >= MAX_BULLET_COUNT)
					return FALSE;
	return TRUE;
}

// bullets will move up by one unit
void BulletMove() {
	unsigned short x, y;
	for(y=2;y<SI_BOARD_HEIGHT-2;y++)
		for(x=1;x<SI_BOARD_WIDTH-1;x++)
			if(SI.board[y][x] == BULLET) {
				SI.board[y-1][x] = BULLET;
				SI.board[y][x] = EMPTY;
			}
}

// erases the bullets that hit the ceiling of the board
void BulletRemove() {
	unsigned short x;
	for(x=1;x<SI_BOARD_WIDTH-1;x++)
		if(SI.board[1][x] == BULLET)
			SI.board[1][x] = EMPTY;
}

// if a bullet collides with a bomb or an enemy, creates an impact 'X'
void BulletImpact() {
	unsigned short x,y;
	for(y=1;y<SI_BOARD_HEIGHT-2;y++)
		for(x=1;x<SI_BOARD_WIDTH-1;x++)
			if(SI.board[y][x] == BULLET)
				switch(SI.board[y-1][x]) {
					case BOMB:
						SI.board[y-1][x] = IMPACT;
						SI.board[y][x] = EMPTY;
						break;
					case ENEMY:
						SI.board[y-1][x] = IMPACT;
						SI.board[y][x] = EMPTY;
						if(IncreaseSIScore())
							return;
				}
}

// erases the impacts from the board
void RemoveImpact() {
	unsigned short x,y;
	for(y=1;y<SI_BOARD_HEIGHT-1;y++)
		for(x=1;x<SI_BOARD_WIDTH-1;x++)
			if(SI.board[y][x] == IMPACT)
				SI.board[y][x] = EMPTY;
}

// an utility function of "BombDrop" that checks if an enemy is below the given point
unsigned short BottomEnemyCheck(unsigned short bomb_x, unsigned short y) {
	unsigned short x;
	for(x=bomb_x-2;x<bomb_x+3;x++)
		if(SI.board[y][x] == ENEMY)
			return FALSE;
	SI.board[y][bomb_x] = BOMB;
	return TRUE;
}

// each enemy can drop a bomb with a %50 chance
void BombDrop(unsigned short bomb_x) {
	unsigned short y;
	for(y=1;y<SI_BOARD_HEIGHT-4;y++)
		if(rand()%2 && SI.board[y][bomb_x] == ENEMY) {
			unsigned short dy;
			for(dy=1;dy<7;dy++)
				if(BottomEnemyCheck(bomb_x, y+dy))
					break;
		}
}

// bombs will move down by one unit
void BombMove() {
	unsigned short x, y;
	for(y=SI_BOARD_HEIGHT-3;y>1;y--)
		for(x=1;x<SI_BOARD_WIDTH-1;x++)
			if(SI.board[y][x] == BOMB) {
				SI.board[y][x] = EMPTY;
				if(SI.board[y+1][x] != ENEMY)		// erases the bomb from board if it hits an enemy
					SI.board[y+1][x] = BOMB;
			}
}

// erases the bombs that hit the floor of the board
void BombRemove() {
	unsigned short x;
	for(x=1;x<SI_BOARD_WIDTH-1;x++)
		if(SI.board[SI_BOARD_HEIGHT-2][x] == BOMB)
			SI.board[SI_BOARD_HEIGHT-2][x] = EMPTY;
}

// if the hero collides with a bomb or an enemy, creates an impact 'X'
unsigned short BombImpact() {
	switch(SI.board[SI_BOARD_HEIGHT-3][SI.ship_x]) {
		case BOMB:
			SI.lives--;
			SI.board[SI_BOARD_HEIGHT-3][SI.ship_x] = EMPTY;
			SI.board[SI_BOARD_HEIGHT-2][SI.ship_x] = IMPACT;
			return TRUE;
		case ENEMY:
			SI.lives--;
			SI.board[SI_BOARD_HEIGHT-3][SI.ship_x] = EMPTY;
			SI.board[SI_BOARD_HEIGHT-2][SI.ship_x] = IMPACT;
			if(IncreaseSIScore())
				return FALSE;
			return TRUE;
		}
	return FALSE;
}

// an utility function of "EnemyMoveLeftRight", returns true if an enemy hits the wall, false otherwise
unsigned short EnemyTurn() {
	unsigned short y;
	for(y=1;y<SI_BOARD_HEIGHT-1;y++)
		if(SI.board[y][1] == ENEMY || SI.board[y][SI_BOARD_WIDTH-2] == ENEMY)
			return TRUE;
	return FALSE;
}

// all enemies move by one unit to either left or right
void EnemyMoveLeftRight() {
	unsigned short x,y;
	static unsigned short move_left_flag = FALSE;
	if(move_left_flag) {
		for(y=1;y<SI_BOARD_HEIGHT-1;y++)
			for(x=1;x<SI_BOARD_WIDTH-1;x++)
				if(SI.board[y][x] == ENEMY) {
					SI.board[y][x] = EMPTY;
					SI.board[y][x-1] = ENEMY;
				}
	}
	else {
		for(y=1;y<SI_BOARD_HEIGHT-1;y++)
			for(x=SI_BOARD_WIDTH-2;x>0;x--)
				if(SI.board[y][x] == ENEMY) {
					SI.board[y][x] = EMPTY;
					SI.board[y][x+1] = ENEMY;
				}
	}
	if(EnemyTurn()) {
		move_left_flag = !move_left_flag;
		EnemyMoveDown();
		for(x=1;x<SI_BOARD_WIDTH-1;x++)
			if(SI.board[SI_BOARD_HEIGHT-2][x] == ENEMY) {
				game_on_flag_3 = FALSE;
				PrintBoard3();
				MoveCursorToXY(36, 5 + SI_BOARD_HEIGHT);
				printf("Invaders have invaded the Earth.");
				MoveCursorToXY(36, 6 + SI_BOARD_HEIGHT);
				printf("Mission failed. We'll get 'em next time...");
				MoveCursorToXY(36, 7 + SI_BOARD_HEIGHT);
				printf("(Press C to continue...)");
				char key;
				do {
					key = getch();
					if(key == ESCAPE_KEY)
						exit(0);
				} while(key!='c' && key!='C');
				system("cls");
				return;
			}
	}
}

// all enemies move down by one unit
void EnemyMoveDown() {
	unsigned short x,y;
	for(y=SI_BOARD_HEIGHT-3;y>0;y--)
		for(x=1;x<SI_BOARD_WIDTH-1;x++)
			if(SI.board[y][x] == ENEMY) {
				SI.board[y][x] = EMPTY;
				SI.board[y+1][x] = ENEMY;
			}
}

// records the scores of the Space Invaders game in "Records"
unsigned short RecordScore3() {
	time_t mytime = time(NULL);
	char player_name[200];
	printf("Enter your name:\n");
	gets(player_name);
	FILE *info = fopen("./Records/space_invaders_record.txt", "a+");
	fprintf(info, "Player Name: %s\n", player_name);
	fprintf(info, "Played Date: %s", ctime(&mytime));
	fprintf(info, "Score: %u\n", SI.score);
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
			info = fopen("./Records/space_invaders_record.txt", "r");
			char ch;
			while((ch = fgetc(info)) != EOF)
				printf("%c", ch);
			fclose(info);
			break;
		case 'R':
		case 'r':
			game_on_flag_3 = TRUE;
			SI = (SpaceInvaders){{{0}}, DEFAULT_SHIP_POSITION, SI_DEFAULT_LIVES, 0};
			system("cls");
			return TRUE;
		case 'M':
		case 'm':
			return FALSE;
	}
	exit(0);
}

// the main function to play Space Invaders
void PlaySpaceInvaders() {
	unsigned short play_again_flag = FALSE;
	do {
		OpeningScreen3();
		LoadingScreen();
		CopySpawnBoard();
		PrintBoard3();
		UpdateFrame3();
		play_again_flag = RecordScore3();
	} while(play_again_flag);
}

#endif
