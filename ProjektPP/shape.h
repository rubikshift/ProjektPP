#pragma once
#include "point.h"

class shape
{
	protected:
		point start;
		point end;
		int color;

		void drawStraightLineOY(point start, point end, int** img = 0);
		void drawStraightLineOX(point start, point end, int** img = 0);
	public:
		virtual void draw(int** img = 0) = 0;

		void setStart(const point* p);
		void setEnd(const point* p);
		void setColor(const int* color);
};
