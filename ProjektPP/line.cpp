#include "line.h"
#include "math.h"
#include "conio2.h"
#include "projektPP.h"

line::line(const point* start, const int* color)
{
	this->setStart(start);
	this->setColor(color);
}

void line::draw(int** img)
{
	if (this->start == this->end || this->start.y == this->end.y || this->start.x == this->end.x)
		this->drawStraightLine(img);
	else
		this->drawDiagonalLine(img);
}

void line::drawStraightLine(int** img)
{
	if (this->start == this->end)
	{
		if(img != 0)
			img[this->start.y - MIN_Y_POSITION][this->start.x - MIN_X_POSITION] = this->color;
		else
		{
			gotoxy(this->start.x, this->start.y );
			textbackground(this->color);
			putch(' ');
		}
	}
	else if (this->start.y == this->end.y)
		this->drawStraightLineOX(this->start, this->end, img);
	else if (this->start.x == this->end.x)
		this->drawStraightLineOY(this->start, this->end, img);
}

void line::drawDiagonalLine(int** img)
{
	if (absolute(this->start.y - this->end.y) > absolute(this->start.x - this->end.x))
		this->drawDiagonalLineOY(img);
	else
		this->drawDiagonalLineOX(img);
	
}

void line::drawDiagonalLineOY(int** img)
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
			this->drawStraightLineOY(this->start, this->end, img);
		}
		else
		{
			if (this->end.y > this->start.y)
				this->end.y = this->start.y + partialLength;
			else
				this->end.y = this->start.y - partialLength;
			this->end.x = this->start.x;

			this->drawStraightLineOY(this->start, this->end, img);

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

void line::drawDiagonalLineOX(int** img)
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
			this->drawStraightLineOX(this->start, this->end, img);
		}
		else
		{
			if (this->end.x > this->start.x)
				this->end.x = this->start.x + partialLength;
			else
				this->end.x = this->start.x - partialLength;
			this->end.x = this->start.x;

			this->drawStraightLineOX(this->start, this->end, img);

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

