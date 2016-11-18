#include "conio2.h"
#include "rectangle.h"
#include "const.h"

rectangle::rectangle(const point* start, const int* color)
{ this->setStart(start); this->setColor(color); }

void rectangle::draw(int** img)
{
	if (this->start == this->end)
		this->setPixel(this->start.y, this->start.x, this->color, img);
	else
	{
		point p1 = this->start, p2 = { this->start.x, this->end.y }, p3 = this->end, p4 = { this->end.x, this->start.y };
		drawStraightLineOX(p1, p4, img);
		drawStraightLineOY(p1, p2, img);
		drawStraightLineOX(p2, p3, img);	
		drawStraightLineOY(p3, p4, img);
	}
}