#pragma once
#include "point.h"

class cursor
{
	public:
		point position;
		int actualColor;

		cursor();

		void goToPoint(const point* p);
		void setColor(const int* color);
		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();
		
		point* getPositionPointer();
		int* getColorPointer();
};
