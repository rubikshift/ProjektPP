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
	// je�eli program jest kompilowany w czystym j�zyku C
	// prosz� odkomentowa� poni�sz� lini�
	// Conio2_Init();

	file* f = new file("test", 50, 20);
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
			if(zn == ARROW_UP && y > MIN_Y_POSITION) y--;
			else if(zn == ARROW_DOWN && y < MAX_Y_POSITION) y++;
			else if(zn == ARROW_LEFT && x > MIN_X_POSITION) x--;
			else if(zn == ARROW_RIGHT && x < MAX_X_POSITION) x++;
		}
		else if (inputChangesTextColor(&zn, &attr)) attr = changeColor(&zn, &attr);
		else if(zn == ENTER) back = (back + 1) % 16;
	} while (zn != ESC);

	return 0;
	}
