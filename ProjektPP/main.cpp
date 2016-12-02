#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "conio2.h"
#include "file.h"
#include "interface.h"
#include "BasicFileOperations.h"
//#define DEBUG

const char* UI[] = {"Nazwa pliku: ", "esc = wyjscie, anulowanie rysowania", "strzalki = poruszanie",
	"l = rysowanie linii", "k = rysowanie prostokata", "0123456789qwerty = wybor koloru", "s = zapisywanie pliku",
	"i = wczytywanie domyslnego pliku",	"o = wczytywanie pliku", "n = tworzenie nowego pliku", "backspace = cofnij",
	"PageDown = poprzedni plik", "PageUp = nastepny plik", "X: ", "Y: ", "Tryb: ", "Nazwa: ", "Szerokosc: ",
	"Wysokosc: ", "Czy chcesz zmienic nazwe? (t/n)"};

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
			if (*acctualColor == BLACK) return WHITE;
			else return BLACK; break;
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

	for (int i = 0; i < CONST_UI_ELEMENTS; i++)
	{
		textcolor(LIGHTGRAY);
		if (actualFile != NULL)
		{
			if (i == fileInfo)
			{
				gotoxy(UI_X_POSITION + strlen(UI[i]), i + 1);
				cputs(actualFile->getFileName());
			}
			else if ((i == undo && !actualFile->isUndoEnable()) || (i == prevFile && actualId == 0) || (i == nextFile && actualId == fileCounter-1))
				textcolor(DARKGRAY);
			else if (i == xPosition)
			{
				gotoxy(UI_X_POSITION + strlen(UI[i]), i + 1);
				itoa(actualFile->localCursor->position.x - MIN_X_POSITION, buff, 10);
				cputs(buff);
			}
			else if (i == yPosition)
			{
				gotoxy(UI_X_POSITION + strlen(UI[i]), i + 1);
				itoa(actualFile->localCursor->position.y - MIN_Y_POSITION, buff, 10);
				cputs(buff);
			}
			else if (i == actualMode)
				switch (actualFile->getDrawingMode())
				{
					case drawLine: gotoxy(UI_X_POSITION + strlen(UI[i]), i + 1); cputs("Rysowanie linii"); break;
					case drawRectangle: gotoxy(UI_X_POSITION + strlen(UI[i]), i + 1); cputs("Rysowanie prostokata"); break;
					case none: textcolor(DARKGRAY); break;
				}
		}
		gotoxy(UI_X_POSITION, i + 1);
		cputs(UI[i]);
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
	gotoxy(UI_X_POSITION, 17);
	cputs(txt);
#endif // DEBUG
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
				gotoxy(UI_X_POSITION, (int)question);
				cputs(UI[question]);
				input = getch();
				if (input == 't' || input == 'T')
				{
					char* fileName = inputFileName();
					openedFiles[actualFileId]->saveFile(fileName);
					delete[] fileName;
				}
				else openedFiles[actualFileId]->saveFile();
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
