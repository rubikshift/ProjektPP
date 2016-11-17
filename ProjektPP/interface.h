#pragma once
#include "projektPP.h"
#include "point.h"

struct interface
{
	char* text;
	point position;
};

const interface UI[UI_ELEMENTS] =
{
	{ "Nazwa pliku: ",{ UI_X_POSITION, FILE_NAME_INFO } },
	{ "esc = wyjscie, anulowanie rysowania",{ UI_X_POSITION, EXIT_INFO } },
	{ "strzalki = poruszanie",{ UI_X_POSITION, MOVE_INFO } },
	{ "l = rysowanie linii",{ UI_X_POSITION, DRAW_LINE_INFO } },
	{ "k = rysowanie prostokata",{ UI_X_POSITION, DRAW_RECTANGLE_INFO } },
	{ "0123456789qwerty = wybor koloru",{ UI_X_POSITION, CHOSE_COLOR_INFO } },
	{ "s = zapisywanie pliku",{ UI_X_POSITION, SAVE_FILE_INFO } },
	{ "i = wczytywanie domyslnego pliku",{ UI_X_POSITION, LOAD_DEFAULT_FILE_INFO } },
	{ "o = wczytywanie pliku",{ UI_X_POSITION, LOAD_FILE_INFO } },
	{ "n = tworzenie nowego pliku",{ UI_X_POSITION, CREATE_NEW_FILE_INFO } },
	{ "backspace = cofnij",{ UI_X_POSITION, UNDO_INFO } },
	{ "PageDown = poprzedni plik",{ UI_X_POSITION, PREV_FILE_INFO } },
	{ "PageUp = nastepny plik",{ UI_X_POSITION, NEXT_FILE_INFO } },
	{ "X: ",{ UI_X_POSITION, CURSOR_X_POSITION_INFO } },
	{ "Y: ",{ UI_X_POSITION, CURSOR_Y_POSITION_INFO } },
	{ "Tryb: ",{ UI_X_POSITION, MODE_INFO } },
	{ "Tryb: Rysowanie linii",{ UI_X_POSITION, MODE_INFO } },
	{ "Tryb: Rysowanie prostokata",{ UI_X_POSITION, MODE_INFO } },
	{ "Nazwa: ",{ UI_X_POSITION, INPUT_FILE_NAME } },
	{ "Szerokosc: ",{ UI_X_POSITION, INPUT_FILE_WIDTH } },
	{ "Wysokosc: ",{ UI_X_POSITION, INPUT_FILE_HEIGHT } },
	{ "Czy chcesz zmienic nazwe? (t/n)",{ UI_X_POSITION, QUESTION } }
};