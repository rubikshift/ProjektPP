#pragma once
#include "point.h"

class shape
{
	protected:
		point start;
		point end;
		int color;
	public:
		virtual void draw() = 0;
		virtual void draw(int** img) = 0;
		virtual void setStart(point* p) = 0;
		virtual void setEnd(point* p) = 0;
};