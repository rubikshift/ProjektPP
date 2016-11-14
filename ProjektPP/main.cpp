#include <stdlib.h>
#include <string.h>
#include "projektPP.h"
#include "conio2.h"
#include "file.h"

namespace UI
{
	char* text[] = { "Michal Krakowiak 165596", "esc = wyjscie", "strzalki = poruszanie", "spacja = zmiana koloru", "enter = zmiana koloru tla",
		"l = rysowanie linii", "0123456789qwerty = wybor koloru" };
	int shift = strlen(text[CHANGE_BACKGROUND_COLOR_INFO]) + 1;
}


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

void drawUI(const int* zn, const int* zero)
{
	char txt[32] = "kod klawisza: 0x";
	textbackground(BLACK);
	clrscr();
	textcolor(LIGHTGRAY);

	gotoxy(1, 1);
	cputs(UI::text[EXIT_INFO]);
	
	gotoxy(1 + UI::shift, 1);
	cputs(UI::text[DRAW_LINE_INFO]);

	gotoxy(1, 2);
	cputs(UI::text[MOVE_INFO]);
	
	gotoxy(1 + UI::shift, 2);
	cputs(UI::text[CHOSE_TEXT_COLOR_INFO]);

	gotoxy(1, 3);
	cputs(UI::text[CHANGE_TEXT_COLOR_INFO]);
	
	gotoxy(1, 4);
	cputs(UI::text[CHANGE_BACKGROUND_COLOR_INFO]);
	
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
	gotoxy(1, 5);
	cputs(txt);
}

int main(int argc, char** argv) {
	int input = 0, zero = 0;
	bool toClose = false;
	// je¿eli program jest kompilowany w czystym jêzyku C
	// proszê odkomentowaæ poni¿sz¹ liniê
	// Conio2_Init();

	file* f;

	if(argc == 1)
		f = new file("test.xpm", 50, 20);	
	if (argc == 2)
		f = new file(argv[1]);
	settitle(UI::text[WINDOW_TITLE]);

	do 
	{
		drawUI(&input, &zero);
		f->updateView();
		zero = 0;
		input = getch();
		if(input == 0)
		{
			zero = 1;
			input = getch();
			if (input == ARROW_UP) f->localCursor->moveUp();
			else if (input == ARROW_DOWN) f->localCursor->moveDown();
			else if (input == ARROW_LEFT) f->localCursor->moveLeft();
			else if (input == ARROW_RIGHT) f->localCursor->moveRight();
		}
		else if (inputChangesTextColor(&input, f->localCursor->getColorPointer())) f->localCursor->setColor(changeColor(&input, f->localCursor->getColorPointer()));
		else if ((input == 'l' || input == 'L') && f->isInteractiveModeEnabled() != true) f->addLine();
		else if ((input == 'k' || input == 'K') && f->isInteractiveModeEnabled() != true) f->addRectangle();
		else if ((input == 'k' || input == 'K' || input == 'l' || input == 'L') && f->isInteractiveModeEnabled() == true) f->finishDrawing();
		else if (f->isInteractiveModeEnabled() == true && input == ESC) f->cancelDrawing();
		else if (f->isInteractiveModeEnabled() != true && input == BACKSPACE) f->undoLastAction();
		else if (f->isInteractiveModeEnabled() == false && input == ESC) toClose = true;
		else if (f->isInteractiveModeEnabled() == false && input == 's') f->saveFile();
	} while (!toClose);

	delete f;
	return 0;
}
