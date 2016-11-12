#include "rectangle.h"

rectangle::rectangle(const point* start, const int* color)
{
	this->setStart(start);
	this->setColor(color);
}

void rectangle::draw(short int** img)
{
	int startY, endY, startX, endX;
	if(this->start == this->end)
		img[this->start.y][this->start.x] = this->color;
	else
	{
		if (this->start.y < this->end.y)
		{
			startY = this->start.y;
			endY = this->end.y;
		}
		else
		{
			startX = this->end.x;
			endX = this->start.x;
		}
		
		if (this->start.x < this->end.x)
		{
			startX = this->start.x;
			endX = this->end.x;
		}
		else
		{
			startX = this->end.x;
			endX = this->start.x;
		}
		for (int y = startY; y < endY; y++)
			for (int x = startX; x < endX; x++)
					img[y][x] = this->color;
	}
}
