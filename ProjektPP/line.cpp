#include "line.h"

line::line(const point* start, const int* color)
{
	this->start = *start;
	this->color = *color;
}

void line::draw()
{
	
}

void line::draw(int** img)
{
	if (this->start == this->end)
		img[this->start.y][this->start.x] = this->color;
	else if (this->start.y == this->end.y)
	{
		if (this->start.x < this->end.x)
		{
			int startX = this->start.x;
			int endX = this->end.x;
		}
		else
		{
			int startX = this->end.x;
			int endX = this->start.x;
		}
		
		for (int x = startX; x < endX; x++)
			img[this->start.y][x] = color;
	}
	else if (this->start.x == this->end.x)
	{
		if (this->start.y < this->end.y)
		{
			int startY = this->start.y;
			int endY = this->end.y;
		}
		else
		{
			int startY = this->end.y;
			int endY = this->start.y;
		}
		
		for (int y = startY; y < endY; y++)
			img[y][this->start.x] = color;
	}
	else
	{
		
	}
}

void line::setStart(const point* p)
{
	this->start = *p;
}

void line::setEnd(const point* p)
{
	this->end = *p;
}

void line::setColor(const int* color)
{
	this->color = *color;
}
