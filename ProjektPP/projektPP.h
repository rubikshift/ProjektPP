#pragma once

/*STALE KLAWISZOWE*/
#define ESC 0x1b
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4b
#define ARROW_RIGHT 0x4d
#define ENTER 0x0d

/*STALE INTERFEJSU*/
#define WINDOW_TITLE 0
#define EXIT_INFO 1
#define MOVE_INFO 2
#define CHANGE_TEXT_COLOR_INFO 3
#define CHANGE_BACKGROUND_COLOR_INFO 4
#define DRAW_LINE_INFO 5
#define CHOSE_TEXT_COLOR_INFO 6

#define MIN_X_POSITON 1
#define MAX_X_POSITION 120
#define MIN_Y_POSITON 6
#define MAX_Y_POSITION 30

namespace UI
{
	char* text[] = { "Michal Krakowiak 165596", "esc = wyjscie", "strzalki = poruszanie", "spacja = zmiana koloru", "enter = zmiana koloru tla",
				"l = rysowanie linii", "0123456789qwerty = wybor koloru" };
	int shift = strlen(text[CHANGE_BACKGROUND_COLOR_INFO]) + 1;
}
