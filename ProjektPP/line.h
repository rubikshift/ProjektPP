#pragma once
#include "shape.h"

class line : public shape
{
	private:
		void drawStraightLine(int** img = NULL);

		void drawDiagonalLine(int** img = NULL);
		void drawDiagonalLineOX(int** img = NULL);
		void drawDiagonalLineOY(int** img = NULL);
	public:
		line(const point* start, const int* color);
		
		void draw(int** img = 0);		
};
