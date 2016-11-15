#include "conio2.h"
#include "rectangle.h"
#include "projektPP.h"

rectangle::rectangle(const point* start, const int* color)
{
	this->setStart(start);
	this->setColor(color);
}

void rectangle::draw(int** img)
{
	if(this->start == this->end)
	{
		if(img != 0)
			img[this->start.y - MIN_Y_POSITION][this->start.x - MIN_X_POSITION] = this->color;
		else
		{
			gotoxy(this->start.x, this->start.y);
			textbackground(this->color);
		}
	}
	else
	{
		point p1 = this->start, p2 = { this->start.x, this->end.y }, p3 = this->end, p4 = { this->end.x, this->start.y };
		drawStraightLineOX(p1, p4, img);
		drawStraightLineOY(p1, p2, img);
		drawStraightLineOX(p2, p3, img);	
		drawStraightLineOY(p3, p4, img);
	}
}
