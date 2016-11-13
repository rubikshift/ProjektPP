#include "cursor.h"
#include "conio2.h"
#include "projektPP.h"

cursor::cursor(int maxXPosition, int maxYPosition)
{
	this->position = { MIN_X_POSITION, MIN_Y_POSITION };
	this->actualColor = LIGHTRED;
	this->maxXPosition = MIN_X_POSITION + maxXPosition - 1;
	this->maxYPosition = MIN_Y_POSITION + maxYPosition - 1;
	this->cursorCharacter = '*';
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
	if (this->position.y < this->maxYPosition)
		this->position.y++;
}

void cursor::moveLeft()
{
	if (this->position.x > MIN_X_POSITION)
		this->position.x--;
}

void cursor::moveRight()
{
	if (this->position.x < this->maxXPosition)
		this->position.x++;
}

point* cursor::getPositionPointer()
{
	return &(this->position);
}

int* cursor::getColorPointer()
{
	return &(this->actualColor);
}
