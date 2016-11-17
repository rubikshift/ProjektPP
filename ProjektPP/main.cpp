#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "projektPP.h"
#include "conio2.h"
#include "file.h"
#include "interface.h"

#define _CRT_SECURE_NO_WARNINGS

//#define DEBUG

int changeColor(const int* input, const int* acctualColor)
{
	switch (*input)
	{
	case '0': return BLACK;
	case '1': return BLUE;
	case '2': return GREEN;
	case '3': return CYAN;
	case '4': return RED;
	case '5': return MAGENTA;
	case '6': return BROWN;
	case '7': return LIGHTGRAY;
	case '8': return DARKGRAY;
	case '9': return LIGHTBLUE;
	case 'q':
	case 'Q': return LIGHTGREEN;
	case 'w':
	case 'W': return LIGHTCYAN;
	case 'e':
	case 'E': return LIGHTRED;
	case 'r':
	case 'R': return LIGHTMAGENTA;
	case 't':
	case 'T': return YELLOW;
	case 'y':
	case 'Y': return WHITE;
	case ' ':
	default: return *acctualColor;
	}
}

bool inputChangesTextColor(const int* input, const int* acctualColor)
{
	if (changeColor(input, acctualColor) != *acctualColor)
		return true;
	else
		return false;
}

void drawUI(const int* zn, const int* zero, int fileCounter, int actualId, file* actualFile = NULL)
{
	char buff[BUFF];
	textbackground(BLACK);
	clrscr();

	for (int i = 0; i < UI_ELEMENTS; i++)
	{
		textcolor(LIGHTGRAY);
		if (actualFile != NULL)
			if (UI[i].position.y == UNDO_INFO || (UI[i].position.y == PREV_FILE_INFO && actualId == 0) || (UI[i].position.y == NEXT_FILE_INFO && actualId == fileCounter - 1))
				if (actualFile->isUndoEnable() != true)
					textcolor(DARKGRAY);
		if (i < MODE_INFO - 1)
		{
			gotoxy(UI[i].position.x, UI[i].position.y);
			cputs(UI[i].text);
		}
		if (UI[i].position.y == FILE_NAME_INFO && actualFile != NULL)
		{
			gotoxy(UI[i].position.x + strlen(UI[i].text), UI[i].position.y);
			cputs(actualFile->getFileName());
		}
		else if ((UI[i].position.y == CURSOR_X_POSITION_INFO) && actualFile != NULL)
		{
			gotoxy(UI[i].position.x + strlen(UI[i].text), UI[i].position.y);
			itoa(actualFile->localCursor->position.x - MIN_X_POSITION + 1, buff, 10);
			cputs(buff);
			memset(buff, 0, strlen(buff));
		}
		else if ((UI[i].position.y == CURSOR_Y_POSITION_INFO) && actualFile != NULL)
		{
			gotoxy(UI[i].position.x + strlen(UI[i].text), UI[i].position.y);
			itoa(actualFile->localCursor->position.y - MIN_Y_POSITION + 1, buff, 10);
			cputs(buff);
			memset(buff, 0, strlen(buff));
		}
		else if ((UI[i].position.y == MODE_INFO) && actualFile != NULL)
		{
			if (actualFile->getDrawingMode() == none && i == MODE_INFO - 1)
			{
				textcolor(DARKGRAY);
				gotoxy(UI[i].position.x, UI[i].position.y);
				cputs(UI[i].text);
				break;
			}
			if (actualFile->getDrawingMode() == drawLine && i == LINE_MODE_INFO - 1)
			{
				gotoxy(UI[i].position.x, UI[i].position.y);
				cputs(UI[i].text);
				break;
			}
			if (actualFile->getDrawingMode() == drawRectangle && i == RECTANGLE_MODE_INFO - 1)
			{
				gotoxy(UI[i].position.x, UI[i].position.y);
				cputs(UI[i].text);
				break;
			}
			else continue;
		}
	}
#ifdef DEBUG
	char txt[32] = "kod klawisza: 0x";
	if (*zero) {
		txt[16] = '0';
		txt[17] = '0';
		txt[18] = ' ';
		txt[19] = '0';
		txt[20] = 'x';
		itoa(*zn, txt + 21, 16);
	}
	else {
		itoa(*zn, txt + 16, 16);
	}
	textcolor(LIGHTGRAY);
	gotoxy(UI_X_POSITION, MODE_INFO + 2);
	cputs(txt);
#endif // DEBUG
}

char* inputFileName()
{
	char* fileName = new char[MAX_FILENAME_LENGTH];
	int input, n = 0;
	memset(fileName, 0, MAX_FILENAME_LENGTH);
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	gotoxy(UI[UI_ELEMENTS - 4].position.x, UI[UI_ELEMENTS - 4].position.y);
	cputs(UI[UI_ELEMENTS - 4].text);
	bool toStop = false;
	do
	{
		input = getch();
		if (n < MAX_FILENAME_LENGTH && ((input >= '0' && input <= '9') || (input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z') || input == '.' || input == '-' || input == '_'))
		{
			gotoxy(UI[UI_ELEMENTS - 4].position.x + strlen(UI[UI_ELEMENTS - 4].text) + n, UI[UI_ELEMENTS - 4].position.y);
			putch((char)input);
			fileName[n] = (char)input;
			n++;
		}
		if (input == ENTER && n > 0)
			toStop = true;
	} while (toStop != true);
	return fileName;
}

int inputHeightWidth(interface UItoPrint)
{
	char buff[BUFF];
	memset(buff, 0, BUFF);
	int input = 0, n = 0, x;
	bool toStop = false;
	gotoxy(UItoPrint.position.x, UItoPrint.position.y);
	cputs(UItoPrint.text);
	while (toStop != true)
	{
		input = getch();
		if (n < BUFF && input >= '0' && input <= '9')
		{
			gotoxy(UItoPrint.position.x + strlen(UItoPrint.text) + n, UItoPrint.position.y);
			putch((char)input);
			buff[n] = (char)input;
			n++;
		}
		if (input == ENTER && n > 0)
			toStop = true;
	}
	x = atoi(buff);
	return x;
}

void openNewFile(file** actualFile, int &actualFileId, int &fileCounter)
{
	int width, height;
	char* fileName;
	textbackground(BLACK);
	textcolor(LIGHTGRAY);
	fileName = inputFileName();
	width = inputHeightWidth(UI[UI_ELEMENTS - 3]);
	height = inputHeightWidth(UI[UI_ELEMENTS - 2]);
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
	gotoxy(UI[UI_ELEMENTS - 4].position.x, UI[UI_ELEMENTS - 4].position.y);
	cputs(UI[UI_ELEMENTS - 4].text);
	fileName = inputFileName();
	if (fopen(fileName, "r") != NULL)
	{
		openedFiles[fileCounter] = new file(fileName);
		actualFileId = fileCounter;
		fileCounter++;
	}
	delete[] fileName;
}

void init(file** openedFiles, int &actualFileId, int &fileCounter)
{
	int input;
	while (fileCounter == 0)
	{
		drawUI(&input, &input, fileCounter, actualFileId);
		input = getch();
		switch (input)
		{
		case 'i':
		case 'I':
			openDefaultFile(openedFiles, actualFileId, fileCounter); break;
		case 'o':
		case 'O':
			openFile(openedFiles, actualFileId, fileCounter); break;
		case 'n':
		case 'N':
			openNewFile(openedFiles, actualFileId, fileCounter); break;
		default: break;
		}
	}
}

int main(int argc, char** argv) {
	int input = 0, zero = 0;
	bool toClose = false;
	int fileCounter = 0, fileNumberMultipler = 1, actualFileId = 0;
	file** openedFiles = new file*[DEFAULT_NUMBER_OF_FILES];
	// Conio2_Init();
	settitle("Michal Krakowiak 165596");
	textmode(FULLSCREEN);

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			openedFiles[i - 1] = new file(argv[i]);
			fileCounter++;
			if (fileCounter == DEFAULT_NUMBER_OF_FILES*fileNumberMultipler)
				resizeFileTab(openedFiles, fileCounter, fileNumberMultipler);
		}
		actualFileId = 0;
	}

	init(openedFiles, actualFileId, fileCounter);
	do
	{
		drawUI(&input, &zero, fileCounter, actualFileId, openedFiles[actualFileId]);
		openedFiles[actualFileId]->updateView();
		zero = 0;
		input = getch();
		switch (input)
		{
		case 0: zero = 1;
			input = getch();
			switch (input)
			{
			case ARROW_UP: openedFiles[actualFileId]->localCursor->moveUp(); break;
			case ARROW_DOWN: openedFiles[actualFileId]->localCursor->moveDown(); break;
			case ARROW_LEFT: openedFiles[actualFileId]->localCursor->moveLeft(); break;
			case ARROW_RIGHT: openedFiles[actualFileId]->localCursor->moveRight(); break;
			case PAGEUP:
				if (openedFiles[actualFileId]->isInteractiveModeEnabled() == false && actualFileId < fileCounter - 1) actualFileId++;
				break;
			case PAGEDOWN:
				if (actualFileId > 0) actualFileId--;
				break;
			} break;
		case 'l':
		case 'L':
			if (!openedFiles[actualFileId]->isInteractiveModeEnabled()) openedFiles[actualFileId]->addLine();
			else openedFiles[actualFileId]->finishDrawing();
			break;
		case 'k':
		case 'K':
			if (!openedFiles[actualFileId]->isInteractiveModeEnabled()) openedFiles[actualFileId]->addRectangle();
			else openedFiles[actualFileId]->finishDrawing();
			break;
		case 's':
		case 'S':
			textbackground(BLACK);
			textcolor(LIGHTGRAY);
			gotoxy(UI[UI_ELEMENTS - 1].position.x, UI[UI_ELEMENTS - 1].position.y);
			cputs(UI[UI_ELEMENTS - 1].text);
			input = getch();
			if (input == 't' || input == 'T')
			{
				char* fileName = inputFileName();
				openedFiles[actualFileId]->saveFile(fileName);
				delete[] fileName;
			}
			else
				openedFiles[actualFileId]->saveFile();
			break;
		case 'i':
		case 'I':
			if (!openedFiles[actualFileId]->isInteractiveModeEnabled()) openDefaultFile(openedFiles, actualFileId, fileCounter);
			break;
		case 'n':
		case 'N':
			if (!openedFiles[actualFileId]->isInteractiveModeEnabled()) openNewFile(openedFiles, actualFileId, fileCounter);
			break;
		case 'o':
		case 'O':
			if (!openedFiles[actualFileId]->isInteractiveModeEnabled()) openFile(openedFiles, actualFileId, fileCounter);
			break;
		case BACKSPACE: openedFiles[actualFileId]->undoLastAction(); break;
		case ESC:
			if (openedFiles[actualFileId]->isInteractiveModeEnabled()) openedFiles[actualFileId]->cancelDrawing();
			else toClose = true;
			break;
		default:
			if (inputChangesTextColor(&input, openedFiles[actualFileId]->localCursor->getColorPointer()))
				openedFiles[actualFileId]->localCursor->setColor(changeColor(&input, openedFiles[actualFileId]->localCursor->getColorPointer()));
			break;

		}
		if (fileCounter == DEFAULT_NUMBER_OF_FILES*fileNumberMultipler)
			resizeFileTab(openedFiles, fileCounter, fileNumberMultipler);
	} while (!toClose);

	for (int i = 0; i < fileCounter; i++)
		delete openedFiles[i];
	delete[] openedFiles;
	return 0;
}
