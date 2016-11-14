#pragma once
#include "shape.h"

class line : public shape
{
	private:
		void drawStraightLine(int** img = 0);

		void drawDiagonalLine(int** img = 0);
		void drawDiagonalLineOX(int** img = 0);
		void drawDiagonalLineOY(int** img = 0);
	public:
		line(const point* start, const int* color);
		
		void draw(int** img = 0);		
};
