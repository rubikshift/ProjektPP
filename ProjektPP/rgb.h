#pragma once
#include "projektPP.h"

struct rgb
{
	unsigned char r, g, b;
};

const rgb colors[] = {
	{0, 0 ,0},			//BLACK
	{0, 0, 170},		//BLUE
	{0, 170, 0},		//GREAN
	{0, 170, 170},		//CYAN
	{170, 0, 0},		//RED
	{170, 0 , 170},		//MAGENTA
	{170, 85 , 0},		//BROWN
	{170, 170, 170},	//LIGHTGRAY
	{85, 85, 85},		//DARKGRAY
	{85, 85, 255},		//LIGHTBLUE
	{85, 255, 85},		//LIGHTGREAN
	{85, 255, 255},		//LIGHTCYAN
	{255, 85, 85},		//LIGHTRED
	{255, 85, 255},		//LIGHTMAGENTA
	{255, 255, 85},		//YELLOW
	{255, 255, 255},	//WHITE
};
