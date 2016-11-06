#pragma once
#include "shape.h"

class line : public shape
{
	public:
		void draw(int** img);
		void setStart(const point* p);
		void setEnd(const point* p);
};