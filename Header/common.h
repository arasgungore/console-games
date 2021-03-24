#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/time.h>
#include <windows.h>

#define TRUE 1
#define FALSE 0

#define LEFT_KEY 75		// ASCII codes corresponding to certain keys
#define RIGHT_KEY 77
#define UP_KEY 72
#define DOWN_KEY 80
#define ESCAPE_KEY 27
#define PAUSE_KEY 'p'

void MoveCursorToXY(unsigned short x, unsigned short y);
void LoadingScreen();

// moves the cursor to a different (x, y) location on the terminal
void MoveCursorToXY(unsigned short x, unsigned short y) {
	COORD coord = (COORD){x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// prints a loading screen with a progress bar
void LoadingScreen() {
	MoveCursorToXY(36, 14);
	printf("Loading...");
	MoveCursorToXY(30, 15);
	unsigned short i;
	for(i=1;i<=20;i++) {
		Sleep(100);
		printf("%c", 178);
	}
	system("cls");
}

#endif
