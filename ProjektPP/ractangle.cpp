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
	int startY, endY, startX, endX;
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
		/*if (this->start.y < this->end.y)
		{
			startY = this->start.y;
			endY = this->end.y;
		}
		else if (this->start.y > this->end.y)
		{
			startY = this->end.x;
			endY = this->start.x;
		}
		else
		{
			this->drawStraightLineOX(img);
			return;
		}
		
		if (this->start.x < this->end.x)
		{
			startX = this->start.x;
			endX = this->end.x;
		}
		else if (this->start.x > this->end.x)
		{
			startX = this->end.x;
			endX = this->start.x;
		}
		else
		{
			this->drawStraightLineOY(img);
			return;
		}

		for (int y = startY; y < endY; y++)
			for (int x = startX; x < endX; x++)
			{
				if (img != 0)
					img[y - MIN_Y_POSITION][x - MIN_X_POSITION] = this->color;
				else
				{
					gotoxy(x, y);
					textbackground(this->color);
					putch(' ');
				}
			}*/
		point p1 = this->start, p2 = { this->start.x, this->end.y }, p3 = this->end, p4 = { this->end.x, this->start.y };
		drawStraightLineOX(p1, p4, img);
		drawStraightLineOY(p1, p2, img);
		drawStraightLineOX(p2, p3, img);	
		drawStraightLineOY(p3, p4, img);
	}
}
