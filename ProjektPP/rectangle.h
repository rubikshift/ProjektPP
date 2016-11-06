#pragma once
#include "shape.h"

class rectangle : public shape
{
	public:
		void draw(int** img);
		void setStart(point* p);
		void setEnd(point* p);
};