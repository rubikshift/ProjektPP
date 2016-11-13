#pragma once
#include "point.h"

class cursor
{
	public:
		point position;
		char cursorCharacter;
		int actualColor;

		int maxXPosition;
		int maxYPosition;

		cursor(int maxXPosition, int maxYPositon);

		void setColor(int color);
		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();
		
		point* getPositionPointer();
		int* getColorPointer();
};
