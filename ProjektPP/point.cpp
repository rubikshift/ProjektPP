#include "point.h"
bool point::operator==(point p)
{
	if (this->x == p.x && this->y == p.y)
		return true;
	else return false;
}

void point::operator=(point p)
{
	this->x = p.x;
	this->y = p.y;
}
