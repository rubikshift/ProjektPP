#include "shape.h"
#include "projektPP.h"
#include "conio2.h"

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

void shape::drawStraightLineOX(point start, point end, int** img)
{
	int startX, endX;
	if (start.x < end.x)
	{
		startX = start.x;
		endX = end.x;
	}
	else
	{
		startX = end.x;
		endX = start.x;
	}

	for (int x = startX; x < endX; x++)
	{
		if (img != 0)
			img[this->start.y - MIN_Y_POSITION][x - MIN_X_POSITION] = this->color;
		else
		{
			gotoxy(x, start.y);
			textbackground(this->color);
			putch(' ');
		}
	}
}

void shape::drawStraightLineOY(point start, point end, int** img)
{
	int startY, endY;
	if (start.y < end.y)
	{
		startY = start.y;
		endY = end.y;
	}
	else
	{
		startY = end.y;
		endY = start.y;
	}

	for (int y = startY; y < endY; y++)
	{
		if (img != 0)
			img[y - MIN_Y_POSITION][start.x - MIN_X_POSITION] = this->color;
		else
		{
			gotoxy(start.x, y);
			textbackground(this->color);
			putch(' ');
		}
	}
}