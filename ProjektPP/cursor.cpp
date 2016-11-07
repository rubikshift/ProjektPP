#include "cursor.h"
#include "conio2.h"

void cursor::goToPoint(const point* p)
{
	gotoxy(p->x, p->y);
}

void cursor::setColor(int color)
{
	this->actualColor = color;
}

void cursor::moveUp()
{
	if (this->y > MIN_Y_POSITION)
		this->y--;
}

void cursor::moveDown()
{
	if (this->y < MAX_Y_POSITON)
		this->y++;
}

void cursor::moveLeft()
{
	if (this->x > MIN_X_POSITION)
		this->x--;
}

void cursor::moveRight()
{
	if (this->x < MAX_X_POSITION)
		this->x++;
}
