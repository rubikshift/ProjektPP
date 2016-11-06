#pragma once
#include "point.h"

class cursor
{
	public:
		point position;
		int actualColor;

		void goToPoint(const point* p);
		void setColor(int color);
};