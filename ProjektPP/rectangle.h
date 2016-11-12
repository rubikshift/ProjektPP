#pragma once
#include "shape.h"

class rectangle : public shape
{
	public:
		rectangle(const point* start, const int* color);
	
		void draw(short int** img = 0);
};
