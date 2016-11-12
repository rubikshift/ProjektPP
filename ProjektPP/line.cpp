#include "line.h"
#include "math.h"

line::line(const point* start, const int* color)
{
	this->setStart(start);
	this->setColor(color);
}

void line::draw(short int** img)
{
	if (this->start == this->end || this->start.y == this->end.y || this->start.x == this->end.x)
		this->drawStraightLine(img);
	else
		this->drawDiagonalLine(img);
}

void line::drawStraightLine(short int** img)
{
	if (this->start == this->end)
		img[this->start.y][this->start.x] = this->color;
	else if (this->start.y == this->end.y)
		this->drawStraightLineOY(img);
	else if (this->start.x == this->end.x)
		this->drawStraightLineOX(img);
}

void line::drawStraightLineOX(short int** img)
{
	int startX, endX;
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

	for (int x = startX; x < endX; x++)
		img[this->start.y][x] = color;
}

void line::drawStraightLineOY(short int** img)
{
	int startY, endY;
	if (this->start.y < this->end.y)
	{
		startY = this->start.y;
		endY = this->end.y;
	}
	else
	{
		startY = this->end.y;
		endY = this->start.y;
	}

	for (int y = startY; y < endY; y++)
		img[y][this->start.x] = color;
}

void line::drawDiagonalLine(short int** img)
{
	if (absolute(this->start.y - this->end.y) > absolute(this->start.x - this->end.x))
		this->drawDiagonalLineOY(img);
	else
		this->drawDiagonalLineOX(img);
	
}

void line::drawDiagonalLineOY(short int** img)
{
	int breakPoints, partialLength;
	point endCopy = this->end, startCopy = this->start;
	breakPoints = absolute(this->start.x - this->end.x);
	partialLength = absolute(this->start.y - this->end.y) / (breakPoints + 1);
	for (int i = 0; i <= breakPoints; i++)
	{
		if (i == breakPoints)
		{
			this->end = endCopy;
			this->drawStraightLineOY(img);
		}
		else
		{
			if (this->end.y > this->start.y)
				this->end.y = this->start.y + partialLength;
			else
				this->end.y = this->start.y - partialLength;
			this->end.x = this->start.x;

			this->drawStraightLineOY(img);

			if (this->end.y > this->start.y)
				this->start.y += (partialLength + 1);
			else
				this->start.y -= (partialLength + 1);
			if (this->start.x < endCopy.x)
				this->start.x++;
			else
				this->start.x--;
		}
	}
}

void line::drawDiagonalLineOX(short int** img)
{
	int breakPoints, partialLength;
	point endCopy = this->end, startCopy = this->start;
	breakPoints = absolute(this->start.y - this->end.y);
	partialLength = absolute(this->start.x - this->end.x) / (breakPoints + 1);
	for (int i = 0; i <= breakPoints; i++)
	{
		if (i == breakPoints)
		{
			this->end = endCopy;
			this->drawStraightLineOX(img);
		}
		else
		{
			if (this->end.x > this->start.x)
				this->end.x = this->start.x + partialLength;
			else
				this->end.x = this->start.x - partialLength;
			this->end.x = this->start.x;

			this->drawStraightLineOX(img);

			if (this->end.x > this->start.x)
				this->start.x += (partialLength + 1);
			else
				this->start.x -= (partialLength + 1);
			if (this->start.y < endCopy.y)
				this->start.y++;
			else
				this->start.y--;
		}
	}
}

