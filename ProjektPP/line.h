#pragma once
#include "shape.h"

class line : public shape
{
	private:
		void drawStraightLine(short int** img = 0);
		void drawStraightLineOY(short int** img = 0);
		void drawStraightLineOX(short int** img = 0);

		void drawDiagonalLine(short int** img = 0);
		void drawDiagonalLineOX(short int** img = 0);
		void drawDiagonalLineOY(short int** img = 0);
	public:
		line(const point* start, const int* color);
		
		void draw(short int** img = 0);		
};
