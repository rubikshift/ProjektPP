#pragma once
#include "point.h"
#ifndef NULL
	#define NULL 0
#endif // !NULL

class shape
{
	protected:
		point start;
		point end;
		int color;

		void drawStraightLineOY(point start, point end, int** img = NULL);
		void drawStraightLineOX(point start, point end, int** img = NULL);
		void setPixel(int y, int x, int color, int** img = NULL);
	public:
		virtual void draw(int** img = NULL) = 0;

		void setStart(const point* p);
		void setEnd(const point* p);
		void setColor(const int* color);
};
