#pragma once
#include "shape.h"

class line : public shape
{
	public:
		line(const point* start, const int* color);
		
		void draw();
		void draw(int** img);
		void setStart(const point* p);
		void setEnd(const point* p);
		void setColor(const int* color);
};
