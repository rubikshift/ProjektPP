#pragma once
#include "point.h"

class shape
{
	protected:
		point start;
		point end;
		int color;
	public:
		virtual void draw(short int** img = 0) = 0;

		void setStart(const point* p);
		void setEnd(const point* p);
		void setColor(const int* color);
};
