#include "shape.h"

void shape::setStart(const point* p)
{
	this->start = *p;
}

void shape::setEnd(const point* p)
{
	this->end = *p;
}

void shape::setColor(const int* color)
{
	this->color = *color;
}