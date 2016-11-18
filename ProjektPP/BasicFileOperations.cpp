#pragma once
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "conio2.h"
#include "const.h"
#include "interface.h"
#include "BasicFileOperations.h"

extern const char* UI[];

char* inputFileName()
{
	char* fileName = new char[MAX_FILENAME_LENGTH];
	int input, n = 0;
	memset(fileName, 0, MAX_FILENAME_LENGTH);
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	gotoxy(UI_X_POSITION, inputFileNamePosition);
	cputs(UI[16]);
	bool toStop = false;
	do
	{
		input = getch();
		if (n < MAX_FILENAME_LENGTH && ((input >= '0' && input <= '9') || (input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || input == '.' || input == '-' || input == '_'))
		{
			gotoxy(UI_X_POSITION + strlen(UI[16]) + n, inputFileNamePosition);
			putch((char)input);
			fileName[n] = (char)input;
			n++;
		}
		else if (input == ENTER && n > 0) toStop = true;
	} while (!toStop);
	return fileName;
}

int inputSize(const char* TextToPrint, int x, int y)
{
	char buff[BUFF];
	memset(buff, 0, BUFF);
	int input = 0, n = 0, value;
	bool toStop = false;
	gotoxy(x, y);
	cputs(TextToPrint);
	do
	{
		input = getch();
		if (n < BUFF && input >= '0' && input <= '9')
		{
			gotoxy(x + strlen(TextToPrint) + n, y);
			putch((char)input);
			buff[n] = (char)input;
			n++;
		}
		if (input == ENTER && n > 0)
			toStop = true;
	} while (!toStop);
	value = atoi(buff);
	return value;
}

void openNewFile(file** actualFile, int &actualFileId, int &fileCounter)
{
	int width, height;
	char* fileName;
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	fileName = inputFileName();
	width = inputSize(UI[17], UI_X_POSITION, inputFileWidth);
	height = inputSize(UI[18], UI_X_POSITION, inputFileHeight);
	actualFile[fileCounter] = new file(fileName, width, height);
	actualFileId = fileCounter;
	fileCounter++;
	delete[] fileName;

}

void resizeFileTab(file** fileTab, int counter, int &multipler)
{
	multipler++;
	file** temp = new file*[DEFAULT_NUMBER_OF_FILES*multipler];
	for (int i = 0; i < counter; i++)
		temp[i] = fileTab[i];
	delete[] fileTab;
	fileTab = temp;
}

void openDefaultFile(file** openedFiles, int &actualFileId, int &fileCounter)
{
	if (fopen("default.bmp", "r") != NULL)
		openedFiles[fileCounter] = new file("default.bmp");
	else if (fopen("default.xpm", "r") != NULL)
		openedFiles[fileCounter] = new file("default.xpm");
	else if (fopen("default.mff", "r") != NULL)
		openedFiles[fileCounter] = new file("default.mff");
	else return;
	actualFileId = fileCounter;
	fileCounter++;
}

void openFile(file** openedFiles, int &actualFileId, int &fileCounter)
{
	char* fileName;
	gotoxy(UI_X_POSITION, inputFileNamePosition);
	cputs(UI[16]);
	fileName = inputFileName();
	if (fopen(fileName, "r") != NULL)
	{
		openedFiles[fileCounter] = new file(fileName);
		actualFileId = fileCounter;
		fileCounter++;
	}
	delete[] fileName;
}
