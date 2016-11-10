#include <string.h>
#include "file.h"

#define DEFAULT_STACK_SIZE 100

file::file(const char* name, int width, int heigth)
{
	int length = strlen(name) + 1;
	this->name = new char[length]
	memcpy(this->name, name, length);
	this->buff = new int*[height];
	for(int i = 0; i < height; i++)
		buff[i] = new int[width];
	stack = new shape*[DEFAULT_STACK_SIZE];
	stackSizeMultipler = 1;
}

file::~file()
{
	delete[] name;
	delete[] buff;
	delete[] stack;
}

void file::undoLastAction()
{
	delete this->stack[stackCounter-1];
	this->stackCounter--;
}

void file::addLine()
{
	if (!(this->interactiveMode))
	{	
		this->stack[stackCounter] = new line(this->localCursor.getPosition(), this->localCursor.getColor() 
		this->stackCounter++;
		interactiveMode = true;
		
		if (stackCounter == DEFAULT_STACK_SIZE*stackSizeMultipler)
			this->resizeStack();
	}	
}

void file::addRectangle()
{
	if (!(this->interactiveMode))
	{	
		this->stack[stackCounter] = new shape(this->localCursor.getPositionPointer(), this->localCursor.getColorPointer()); 
		this->stackCounter++;
		interactiveMode = true;
		
		if (stackCounter == DEFAULT_STACK_SIZE*stackSizeMultipler)
			this->resizeStack();
	}
}

void file::cancelDrawing()
{
	this->undoLastAction();
	interactiveMode = false;
}

void file::finishDrawing()
{
	this->stack[stackCounter-1]->setEnd(this->localCursor.getPositionPointer());
	this->updateBuff();
	interactiveMode = false;
}

bool file::isInteractiveModeEnabled()
{
	return this->interactiveMode;
}

void file::resizeStack()
{
	stackSizeMultipler++;
	shape** temp = new shape[DEFAULT_STACK_SIZE*stackSizeMultipler];
	for (int i = 0; i <stackCounter; i++)
		memcpy(stack[i], temp[i], sizeof(stack[i]);
	delete[] stack;
	stack = temp;
}

void file::updateView()
{
	//Rysowanie buffora;
	if(interactiveMode)
	{
		this->stack[stackCounter-1]->setEnd(this->localCursor.getPositionPointer());
		this->stack[stackCounter-1]->draw();
	}
}

void file::updateBuff()
{
	for(int i = 0; i < stackCounter; i++)
		this->stack[i]->draw(this->buff);
}
