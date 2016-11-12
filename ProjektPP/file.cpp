#include <string.h>
#include <stdio.h>
#include "conio2.h"
#include "file.h"
#include "projektPP.h"
#include "rgb.h"

typedef FILE onDiskFile;

file::file(const char* name, int width, int height)
{
	this->init(name, width, height);
}

void file::init(const char* name, int width, int height)
{
	int length = strlen(name) + 1;
	this->name = new char[length];
	memcpy(this->name, name, length);
	this->img = new short int*[height];
	for (int i = 0; i < height; i++)
	{
		img[i] = new short int[width];
		for (int j = 0; j < width; j++)
			img[i][j] = BLACK;
	}
	stack = new shape*[DEFAULT_STACK_SIZE];
	stackSizeMultipler = 1;
	stackCounter = 0;
	this->height = height;
	this->width = width;
	localCursor = new cursor;
	interactiveMode = false;
}

file::~file()
{
	delete[] name;
	for (int i = 0; i < height; i++)
		delete[] img[i];
	delete[] img;
	for (int i = 0; i < DEFAULT_STACK_SIZE*stackSizeMultipler; i++)
		delete stack[i];
	delete[] stack;
	delete localCursor;
}

void file::loadFile(const char* fileName)
{
	extension fileExtension = this->getFileExtension(fileName);
	switch (fileExtension)
	{
		case bmp: this->loadBmpFile(fileName);  break;
		case xpm2: this->loadXpmFile(fileName); break;
		case mff: this->loadMffFile(fileName); break;
		case undefined: break;
	}
}

void file::loadBmpFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "rb");
	unsigned char info[BMP_HEADER];
	if (fileStream != NULL)
	{
		fread(info, sizeof(unsigned char), BMP_HEADER, fileStream);

		this->width = *(short int*)&info[BMP_WIDTH_INFO];
		this->height = *(short int*)&info[BMP_HEIGHT_INFO];

		this->init(fileName, this->width, this->height);

		unsigned char** data = new unsigned char*[this->height];
		for (int i = 0; i < height; i++)
		{
			data[i] = new unsigned char[COLORS_PER_PIXEL * this->width];
			fread(data[i], sizeof(unsigned char), COLORS_PER_PIXEL * this->width, fileStream);
		}
		fclose(fileStream);

		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (data[y][x * COLORS_PER_PIXEL + BMP_R_POSITION] == colors[i].r
						&& data[y][x * COLORS_PER_PIXEL + BMP_G_POSITION] == colors[i].g
						&& data[y][x * COLORS_PER_PIXEL + BMP_B_POSITION] == colors[i].b)
						this->img[y][x] = i;
		for (int i = 0; i < this->height; i++)
			delete[] data[i];
		delete[] data;
	}
}

void file::loadXpmFile(const char* fileName)
{

}

void file::loadMffFile(const char* fileName)
{

}

void file::saveFile(const char* fileName, extension fileExtension)
{

}

void file::undoLastAction(updateMode mode)
{
	delete this->stack[stackCounter-1];
	this->stackCounter--;
	this->updateImg(mode);
}

void file::addLine()
{
	if (!(this->interactiveMode))
	{	
		this->stack[stackCounter] = new line(this->localCursor->getPositionPointer(), this->localCursor->getColorPointer());
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
		this->stack[stackCounter] = new rectangle(this->localCursor->getPositionPointer(), this->localCursor->getColorPointer()); 
		this->stackCounter++;
		interactiveMode = true;
		
		if (stackCounter == DEFAULT_STACK_SIZE*stackSizeMultipler)
			this->resizeStack();
	}
}

void file::cancelDrawing()
{
	this->undoLastAction(last);
	interactiveMode = false;
}

void file::finishDrawing()
{
	this->stack[stackCounter-1]->setEnd(this->localCursor->getPositionPointer());
	this->updateImg();
	interactiveMode = false;
}

bool file::isInteractiveModeEnabled()
{
	return this->interactiveMode;
}

void file::resizeStack()
{
	stackSizeMultipler++;
	shape** temp = new shape*[DEFAULT_STACK_SIZE*stackSizeMultipler];
	for (int i = 0; i <stackCounter; i++)
		memcpy(temp[i], stack[i], sizeof(stack[i]));
	for (int i = 0; i < DEFAULT_STACK_SIZE * (stackSizeMultipler-1); i++)
		delete stack[i];
	delete[] stack;
	stack = temp;
}

void file::updateView()
{
	//Rysowanie buffora;
	if(interactiveMode)
	{
		this->stack[stackCounter-1]->setEnd(this->localCursor->getPositionPointer());
		this->stack[stackCounter-1]->draw();
	}
}

void file::updateImg(updateMode mode)
{
	switch (mode)
	{
		case last: this->stack[stackCounter - 1]->draw(this->img); break;
		case all:
			for (int i = 0; i < stackCounter; i++)
				this->stack[i]->draw(this->img);
			break;
	}
}

extension file::getFileExtension(const char* fileName)
{
	if (strstr(fileName, ".bmp") != NULL)
		return bmp;
	else if (strstr(fileName, ".xpm") != NULL)
		return xpm2;
	else if (strstr(fileName, ".mff") != NULL)
		return mff;
	else
		return undefined;
}