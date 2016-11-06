#include <stdlib.h>
#include<string.h>

#include "conio2.h"
#include "projektPP.h"
#include "point.h"
#include "shape.h"
#include "line.h"
#include "rectangle.h"

/* Uwaga: w docelowym programie nalezy zadeklarowac odpowiednie
   stale, na przyklad po to, aby wyeliminowac z programu
   wartosci numeryczne umieszczajac w ich miejsce 
   dobrze dobrane identyfikatory */

int changeTextColor(const int* zn, const int* acctualTextColor)
{
	switch (*zn)
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
	default: return *acctualTextColor;
	}
}

bool znChangesTextColor(const int* zn)
{
	switch (*zn)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'q':
	case 'Q':
	case 'w':
	case 'W':
	case 'e':
	case 'E':
	case 'r':
	case 'R':
	case 't':
	case 'T':
	case 'y':
	case 'Y': 
	case ' ': return true;
	default: return false;
	}
}

void drawUI(const int* zn, const int* zero, const int* x, const int* y)
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
	int zn = 0, x = 40, y = 12, attr = 7, back = 0, zero = 0;
	// je¿eli program jest kompilowany w czystym jêzyku C
	// proszê odkomentowaæ poni¿sz¹ liniê
	// Conio2_Init();
	settitle(UI::text[WINDOW_TITLE]);

	do 
	{
		drawUI(&zn, &zero, &x, &y);
		gotoxy(x, y);
		textcolor(attr);
		textbackground(back);
		putch('*');
		zero = 0;
		zn = getch();
		if(zn == 0)
		{
			zero = 1;
			zn = getch();
			if(zn == ARROW_UP && y > MIN_Y_POSITON) y--;
			else if(zn == ARROW_DOWN && y < MAX_Y_POSITION) y++;
			else if(zn == ARROW_LEFT && x > MIN_X_POSITON) x--;
			else if(zn == ARROW_RIGHT && x < MAX_X_POSITION) x++;
		}
		else if (znChangesTextColor(&zn)) attr = changeTextColor(&zn, &attr);
		else if(zn == ENTER) back = (back + 1) % 16;
	} while (zn != ESC);

	return 0;
	}
