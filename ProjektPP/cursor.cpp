#include "cursor.h"
#include "conio2.h"
#include "projektPP.h"

void cursor::goToPoint(const point* p)
{
	this->position = *p;
	gotoxy(p->x, p->y);
}

void cursor::setColor(int color)
{
	this->actualColor = color;
}

void cursor::moveUp()
{
	if (this->position.y > MIN_Y_POSITION)
		this->position.y--;
}

void cursor::moveDown()
{
	if (this->position.y < MAX_Y_POSITION)
		this->position.y++;
}

void cursor::moveLeft()
{
	if (this->position.x > MIN_X_POSITION)
		this->position.x--;
}

void cursor::moveRight()
{
	if (this->position.x < MAX_X_POSITION)
		this->position.x++;
}

point* cursor::getPositionPointer()
{
	return &(this->position);
}

int* cursor::getColorPointer()
{
	return &(this->color);
}
