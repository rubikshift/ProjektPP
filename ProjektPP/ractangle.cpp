#include "rectangle.h"

void rectangle::setStart(const point* p)
{
	this->start = *p;
}

void rectangle::setEnd(const point* p)
{
	this->end = *p;
}
