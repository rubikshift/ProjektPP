#include "line.h"

void line::setStart(const point* p)
{
	this->start = *p;
}

void line::setEnd(const point* p)
{
	this->end = *p;
}
