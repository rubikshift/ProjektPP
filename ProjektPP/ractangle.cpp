#include "rectangle.h"

rectangle::rectangle(const point* start, const int* color)
{
	this->start = *start;
	this->color = *color;
}

void reactangle::draw()
{
	
}

void reactangle::draw(int** img)
{
	if(this->start == this->end)
		img[this->start.y][this->start.y] = this->color;
	else
	{
		if (this->start.y < this->end.y)
		{
			int startY = this->start.y
			int endY = this->end.y
		}
		else
		{
			int startX = this->end.x
			int endX = this->start.x
		}
		
		if (this->start.x < this->end.x)
		{
			int startX = this->start.x
			int endX = this->end.x
		}
		else
		{
			int startX = this->end.x
			int endX = this->start.x
		}
		for (int y = startY; y < endY.y; y++)
			for (int x = startX; x < endX; x++)
					img[y][x] = this->color;
	}
}

void rectangle::setStart(const point* p)
{
	this->start = *p;
}

void rectangle::setEnd(const point* p)
{
	this->end = *p;
}

void rectangle::setColor(const int* color)
{
	this->color = *color;
}
