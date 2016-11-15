#include "fill.h"
#include "projektPP.h"

fill::fill(const point* start, const int* colorToSet, const int* colorToChange, int maxWidth, int maxHeight)
{
	this->setStart(start);
	this->setColor(colorToSet);
	this->colorToChange = *colorToChange;
	this->maxHeight = maxHeight;
	this->maxWidth = maxWidth;
}

void fill::draw(int** img)
{
	int deltaXLeft = 0, deltaXRight = 0, deltaYUp = 0, deltaYDown = 0;
	bool toStop = false, toGoLeft = true, toGoUp = true, toGoRight = true, toGoDown = true;
	if (img == 0)
		return;
	while (toStop == false)
	{		
		toGoLeft = toGoUp = toGoRight = toGoDown = false;
		toStop = true;
		for (int y = this->start.y - MIN_Y_POSITION - deltaYUp; y < this->start.y + deltaYDown; y++)
			for (int x = this->start.x - MIN_X_POSITION - deltaXLeft; x <= this->start.x + deltaXRight; x++)
				if (img[y][x] == this->colorToChange)
				{
					img[y][x] = this->color;
					if (y == this->start.y - MIN_Y_POSITION - deltaYUp)
						toGoUp = true;
					if (y == this->start.y + deltaYDown - 1)
						toGoDown = true;
					if (x == this->start.x - MIN_X_POSITION - deltaXLeft)
						toGoRight = true;
					if (x == this->start.x + deltaXRight)
						toGoLeft = true;
				}
		if (this->start.y + deltaYDown < this->maxHeight)
			deltaYDown++;
		if (this->start.x + deltaXRight < this->maxWidth)
			deltaXRight++;
		if (this->start.y - MIN_Y_POSITION - deltaYUp > 0)
			deltaYUp++;
		if (this->start.x - MIN_X_POSITION - deltaXLeft > 0)
			deltaXLeft++;
		if (!toGoDown && !toGoUp && !toGoLeft && !toGoRight)
			toStop = true;
		if (this->start.y + deltaYDown == this->maxHeight && this->start.x + deltaXRight == this->maxWidth
			&& this->start.x - MIN_X_POSITION - deltaXLeft == 0 && this->start.y - MIN_Y_POSITION - deltaYUp == 0)
			toStop = true;
	}
}
