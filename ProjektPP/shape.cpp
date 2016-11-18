#include "shape.h"
#include "const.h"
#include "conio2.h"

void shape::setStart(const point* p)
{ this->start = *p; }

void shape::setEnd(const point* p)
{ this->end = *p; }

void shape::setColor(const int* color)
{ this->color = *color;}

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

	for (int x = startX; x <= endX; x++)
		this->setPixel(start.y, x, this->color, img);
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

	for (int y = startY; y <= endY; y++)
		this->setPixel(y, start.x, this->color, img);
}

void shape::setPixel(int y, int x, int color, int** img)
{
	if (img != NULL)
		img[y - MIN_Y_POSITION][x - MIN_X_POSITION] = color;
	else
	{
		gotoxy(x, y);
		textbackground(color);
		putch(' ');
	}
}
