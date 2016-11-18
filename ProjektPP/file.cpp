#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "conio2.h"
#include "file.h"
#include "rgb.h"
#include "xpm.h"
#include "mff.h"
#include "bmp.h"
#include "line.h"
#include "rectangle.h"
#include "const.h"

typedef FILE onDiskFile;

file::file(const char* fileName)
{ this->loadFile(fileName); }

file::file(const char* name, int width, int height)
{
	this->init(name, width, height);
	this->buff = NULL;
}

void file::init(const char* name, int width, int height)
{
	int length = strlen(name) + 1;
	this->name = new char[length];
	memcpy(this->name, name, length);
	this->img = new int*[height];
	for (int i = 0; i < height; i++)
	{
		this->img[i] = new int[width];
		for (int j = 0; j < width; j++)
			img[i][j] = WHITE;
	}
	this->stack = new shape*[DEFAULT_STACK_SIZE];
	this->stackSizeMultipler = 1;
	this->stackCounter = 0;
	this->height = height;
	this->width = width;
	this->localCursor = new cursor(this->width, this->height);
	this->interactiveMode = false;
	this->drawingMode = none;
}

file::~file()
{
	delete[] name;
	for (int i = 0; i < height; i++)
		delete[] img[i];
	delete[] img;
	for (int i = 0; i < stackCounter; i++)
		delete stack[i];
	delete[] stack;
	delete localCursor;
	if (buff != NULL)
	{
		for (int i = 0; i < height; i++)
			delete[] buff[i];
		delete[] buff;
	}
}

void file::loadFile(const char* fileName)
{
	extension fileExtension = this->getFileExtension(fileName);
	switch (fileExtension)
	{
	case bmp: this->loadBmpFile(fileName);  break;
	case xpm2: this->loadXpmFile(fileName); break;
	case mff: this->loadMffFile(fileName); break;
	case undefined: return; break;
	}
	this->buff = new int*[this->height];
	for (int y = 0; y < this->height; y++)
	{
		this->buff[y] = new int[this->width];
		for (int x = 0; x < this->width; x++)
			this->buff[y][x] = img[y][x];
	}
	this->localCursor->setColor(WHITE - img[this->localCursor->position.y - MIN_Y_POSITION][this->localCursor->position.x - MIN_X_POSITION]);
}

void file::loadBmpFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "rb");
	unsigned char info[bmpEntireHeaderSize];
	int padding;
	if (fileStream != NULL)
	{
		fread(info, sizeof(char), bmpEntireHeaderSize, fileStream);

		this->init(fileName, *(int*)&info[bmpWidth], *(int*)&info[bmpHeight]);

		padding = bmpRowAddressMultipler - ((this->width*bmpBitsPerPixel / 8) % bmpRowAddressMultipler);

		unsigned char** data = new unsigned char*[this->height];
		for (int i = 0; i < this->height; i++)
		{
			data[i] = new unsigned char[bmpColorsPerPixel * this->width];
			fread(data[i], sizeof(char), bmpColorsPerPixel * this->width, fileStream);
			fseek(fileStream, sizeof(char)*padding, SEEK_CUR);
		}
		fclose(fileStream);

		for (int y = 0, n = this->height - 1; y < this->height && n >= 0; y++, n--)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (data[n][x * bmpColorsPerPixel + bmpRedPosition] == rgbColors[i].r
						&& data[n][x * bmpColorsPerPixel + bmpGreenPosition] == rgbColors[i].g
						&& data[n][x * bmpColorsPerPixel + bmpBluePostion] == rgbColors[i].b)
						this->img[y][x] = i;
		for (int i = 0; i < this->height; i++)
			delete[] data[i];
		delete[] data;
	}
}

void file::loadXpmFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "r");
	char buff[BUFF];
	xpmColor* colors;
	if (fileStream != 0)
	{
		fscanf(fileStream, "%[! XPM\n]", buff);
		int values[xpmNumberOfValues];
		for (int i = 0; i < xpmNumberOfValues; i++)
			fscanf(fileStream, "%d", &values[i]);

		colors = new xpmColor[values[xpmColors]];
		char** data = new char*[values[xpmHeight]];
		for (int i = 0; i < values[xpmHeight]; i++)
			data[i] = new char[values[xpmWidth]];
		this->init(fileName, values[xpmWidth], values[xpmHeight]);

		for (int i = 0; i < values[xpmColors]; i++)
		{
			fscanf(fileStream, "%s", &colors[i].id);
			fscanf(fileStream, "%s", buff);
			fscanf(fileStream, "%s", buff);
			for (int j = 0; j < NUMBER_OF_COLORS; j++)
				if (strstr(buff, hexColor[j]) != NULL || strstr(buff, hexColor[j + NUMBER_OF_COLORS]) != NULL)
					colors[i].color = j;
		}
		for (int i = 0; i < values[xpmHeight]; i++)
			fscanf(fileStream, "%s", data[i]);
		fclose(fileStream);

		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < values[xpmColors]; i++)
					if (data[y][x] == colors[i].id)
						this->img[y][x] = colors[i].color;

		delete[] colors;
		/*for (int i = 0; i < this->height; i++)
		delete[] data[i];*/
		delete[] data;
	}
}

void file::loadMffFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "r");
	char buff[BUFF];
	xpmColor* colors;
	int counter;
	if (fileStream != 0)
	{
		fscanf(fileStream, "%[$MFF\n]", buff);
		int values[mffNumberOfValues];
		for (int i = 0; i < mffNumberOfValues; i++)
			fscanf(fileStream, "%d", &values[i]);

		colors = new xpmColor[values[mffHeight]];
		char** data = new char*[values[mffHeight]];
		for (int i = 0; i < values[mffHeight]; i++)
			data[i] = new char[values[mffWidth]];
		this->init(fileName, values[mffWidth], values[mffHeight]);

		for (int i = 0; i < values[mffColors]; i++)
		{
			fscanf(fileStream, "%s", &colors[i].id);
			fscanf(fileStream, "%s", buff);
			colors[i].color = atoi(buff);
		}

		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width;)
			{
				fscanf(fileStream, "%s", buff);
				counter = atoi(buff);
				fscanf(fileStream, "%s", buff);

				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (colors[i].id == buff[0])
					{
						for (int n = 0; n < counter; n++)
							img[y][x + n] = i;
						x += counter;
					}
			}
		delete[] colors;
		for (int i = 0; i < this->height; i++)
			delete[] data[i];
		delete[] data;
	}
}

void file::saveFile(const char* fileName)
{
	if (fileName == NULL)
		fileName = this->name;
	extension fileExtension = this->getFileExtension(fileName);
	switch (fileExtension)
	{
	case bmp: this->saveBmpFile(fileName);  break;
	case xpm2: this->saveXpmFile(fileName); break;
	case mff: this->saveMffFile(fileName); break;
	case undefined:
		char* newFileName = new char[strlen(fileName) + strlen(".mff") + 1]; break;
		memcpy(newFileName, fileName, strlen(fileName));
		strcat(newFileName, ".mff");
		this->saveFile(newFileName);
		delete[] newFileName;
	}
}

void file::saveBmpFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "wb");
	int padding = bmpRowAddressMultipler - ((this->width*bmpBitsPerPixel / 8) % bmpRowAddressMultipler);

	int buff[] = { this->width*this->height*bmpColorsPerPixel + bmpEntireHeaderSize + padding, 0, bmpOffset, bmpInfoHeaderSize, 
		this->width, this->height, bmpPlanes, bmpBitsPerPixel, bmpCompression, this->width*this->height*bmpColorsPerPixel + padding,
		bmpResolution, bmpResolution, bmpDefaultColorPalette, bmpImportantColor, 0};

	if (fileStream != NULL)
	{
		fwrite("BM", sizeof(char), 2, fileStream);		//Nag³ówek "BM"
		for(int i = 0; i < sizeof(buff)/sizeof(int) - 1; i++)
		{
			if (i == bmpFirstShortIntPostion || i == bmpSecondShortIntPostion)
				fwrite(&buff[i], sizeof(short int), 1, fileStream);
			else fwrite(&buff[i], sizeof(int), 1, fileStream);
		}
		for (int y = this->height - 1; y >= 0; y--)
		{
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (this->img[y][x] == i)
					{
						fwrite(&rgbColors[i].b, sizeof(char), 1, fileStream);
						fwrite(&rgbColors[i].g, sizeof(char), 1, fileStream);
						fwrite(&rgbColors[i].r, sizeof(char), 1, fileStream);
					}
			fwrite(&buff[sizeof(buff) / sizeof(int) - 1], sizeof(char), padding, fileStream);
		}

		fclose(fileStream);
	}
}

void file::saveXpmFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "w");

	if (fileStream != NULL)
	{
		xpmColor* colors = new xpmColor[NUMBER_OF_COLORS];
		unsigned char a = 'a';
		for (int i = 0; i < NUMBER_OF_COLORS; i++, a++)
			colors[i] = { a, i };

		fprintf(fileStream, "%s\n", "! XPM");
		fprintf(fileStream, "%d %d %d %d\n", this->width, this->height, NUMBER_OF_COLORS, 1);
		for (int i = 0; i < NUMBER_OF_COLORS; i++)
			fprintf(fileStream, "%c %s %s\n", colors[i].id, "c", hexColor[i]);
		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
				{
					if (this->img[y][x] == colors[i].color && x < this->width - 1)
						fprintf(fileStream, "%c", colors[i].id);
					else if (this->img[y][x] == colors[i].color && x == this->width - 1)
						fprintf(fileStream, "%c\n", colors[i].id);
				}
		fclose(fileStream);
		delete[] colors;
	}
}

void file::saveMffFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "w");
	int counter = 0;
	int c = 0;
	if (fileStream != NULL)
	{
		xpmColor* colors = new xpmColor[NUMBER_OF_COLORS];
		unsigned char a = 'a';
		for (int i = 0; i < NUMBER_OF_COLORS; i++, a++)
			colors[i] = { a, i };

		fprintf(fileStream, "%s\n", "$MFF");
		fprintf(fileStream, "%d %d %d\n", this->width, this->height, NUMBER_OF_COLORS);
		for (int i = 0; i < NUMBER_OF_COLORS; i++)
			fprintf(fileStream, "%c %d\n", colors[i].id, i);
		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width; x++)
			{
				if (counter == 0)
					for (int i = 0; i < NUMBER_OF_COLORS; i++)
						if (this->img[y][x] == i)
							c = i;
				if (img[y][x] == c)
					counter++;
				if (img[y][x] != c || x == this->width - 1)
				{
					if (x < this->width - 1)
					{
						fprintf(fileStream, "%d %c ", counter, colors[c].id);
						x--;
					}
					else if (x == this->width - 1)
						fprintf(fileStream, "%d %c\n", counter, colors[c].id);
					counter = 0;
				}
			}
		fclose(fileStream);
		delete[] colors;
	}
}

void file::undoLastAction()
{
	if (this->isUndoEnable())
	{
		delete this->stack[stackCounter - 1];
		this->stackCounter--;
		this->updateImg();
	}
}

void file::addLine()
{
	this->stack[stackCounter] = new line(this->localCursor->getPositionPointer(), this->localCursor->getColorPointer());
	this->stackCounter++;
	this->interactiveMode = true;
	if (stackCounter == DEFAULT_STACK_SIZE*(this->stackSizeMultipler))
		this->resizeStack();
	this->drawingMode = drawLine;
}

void file::addRectangle()
{
	this->stack[stackCounter] = new rectangle(this->localCursor->getPositionPointer(), this->localCursor->getColorPointer());
	this->stackCounter++;
	this->interactiveMode = true;
	if (this->stackCounter == DEFAULT_STACK_SIZE*(this->stackSizeMultipler))
		this->resizeStack();
	this->drawingMode = drawRectangle;
}

void file::cancelDrawing()
{
	this->undoLastAction();
	this->interactiveMode = false;
	this->drawingMode = none;
}

void file::finishDrawing()
{
	this->stack[stackCounter - 1]->setEnd(this->localCursor->getPositionPointer());
	this->updateImg();
	this->interactiveMode = false;
	this->drawingMode = none;
}

bool file::isInteractiveModeEnabled()
{ return this->interactiveMode; }

void file::resizeStack()
{
	this->stackSizeMultipler++;
	shape** temp = new shape*[DEFAULT_STACK_SIZE*(this->stackSizeMultipler)];
	for (int i = 0; i < this->stackCounter; i++)
		temp[i] = this->stack[i];
	delete[] this->stack;
	this->stack = temp;
}

void file::updateView()
{
	for (int y = 0; y < this->height; y++)
		for (int x = 0; x < this->width; x++)
		{
			gotoxy(x + MIN_X_POSITION, y + MIN_Y_POSITION);
			textbackground(this->img[y][x]);
			putch(' ');
		}
	
	textbackground(this->img[this->localCursor->position.y - MIN_Y_POSITION][this->localCursor->position.x - MIN_X_POSITION]);
	if (interactiveMode)
	{
		this->stack[stackCounter - 1]->setEnd(this->localCursor->getPositionPointer());
		this->stack[stackCounter - 1]->setColor(this->localCursor->getColorPointer());
		this->stack[stackCounter - 1]->draw();
	}
	textcolor(this->localCursor->actualColor);
	gotoxy(this->localCursor->position.x, this->localCursor->position.y);
	putch(this->localCursor->cursorCharacter);
}

void file::updateImg()
{
	for (int i = 0; i < this->height; i++)
		for (int j = 0; j < this->width; j++)
		{
			if (buff == NULL)
				this->img[i][j] = WHITE;
			else
				this->img[i][j] = buff[i][j];
		}
	for (int i = 0; i < this->stackCounter; i++)
		this->stack[i]->draw(this->img);
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

mode file::getDrawingMode()
{ return this->drawingMode; }

const char* file::getFileName()
{ return this->name; }

bool file::isUndoEnable()
{
	if (this->stackCounter > 0 && this->interactiveMode != true)
		return true;
	else return false;
}