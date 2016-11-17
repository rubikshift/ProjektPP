#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "conio2.h"
#include "file.h"
#include "rgb.h"
#include "xpm.h"
#include "line.h"
#include "rectangle.h"

typedef FILE onDiskFile;

file::file(const char* fileName)
{
	this->loadFile(fileName);
}

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
	unsigned char info[BMP_HEADER];
	int padding;
	if (fileStream != NULL)
	{
		fread(info, sizeof(unsigned char), BMP_HEADER, fileStream);

		this->init(fileName, *(int*)&info[BMP_WIDTH_INFO], *(int*)&info[BMP_HEIGHT_INFO]);

		padding = BPM_ROW_ADDRESS_MULTIPLER - ((this->width*BMP_BIT_COUNT / 8) % BPM_ROW_ADDRESS_MULTIPLER);

		unsigned char** data = new unsigned char*[this->height];
		for (int i = 0; i < this->height; i++)
		{
			data[i] = new unsigned char[BMP_COLORS_PER_PIXEL * this->width];
			fread(data[i], sizeof(unsigned char), BMP_COLORS_PER_PIXEL * this->width, fileStream);
			fseek(fileStream, sizeof(unsigned char)*padding, SEEK_CUR);
		}
		fclose(fileStream);

		for (int y = 0, n = this->height - 1; y < this->height && n >= 0; y++, n--)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (data[n][x * BMP_COLORS_PER_PIXEL + BMP_R_POSITION] == rgbColors[i].r
						&& data[n][x * BMP_COLORS_PER_PIXEL + BMP_G_POSITION] == rgbColors[i].g
						&& data[n][x * BMP_COLORS_PER_PIXEL + BMP_B_POSITION] == rgbColors[i].b)
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
	xpm* colors;
	if (fileStream != 0)
	{
		fscanf(fileStream, "%[! XPM\n]", buff);
		int values[XPM_NUMBER_OF_VALUES];
		for (int i = 0; i < XPM_NUMBER_OF_VALUES; i++)
			fscanf(fileStream, "%d", &values[i]);

		colors = new xpm[values[XPM_COLORS]];
		char** data = new char*[values[XPM_HEIGHT]];
		for (int i = 0; i < values[XPM_HEIGHT]; i++)
			data[i] = new char[values[XPM_WIDTH]];
		this->init(fileName, values[XPM_WIDTH], values[XPM_HEIGHT]);

		for (int i = 0; i < values[XPM_COLORS]; i++)
		{
			fscanf(fileStream, "%s", &colors[i].id);
			fscanf(fileStream, "%s", buff);
			fscanf(fileStream, "%s", buff);
			for (int j = 0; j < NUMBER_OF_COLORS; j++)
				if (strstr(buff, hexColor[j]) != NULL || strstr(buff, hexColor[j + NUMBER_OF_COLORS]) != NULL)
					colors[i].color = j;
		}
		for (int i = 0; i < values[XPM_HEIGHT]; i++)
		{
			fscanf(fileStream, "%s", data[i]);
		}
		fclose(fileStream);

		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < values[XPM_COLORS]; i++)
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
	xpm* colors;
	int counter;
	if (fileStream != 0)
	{
		fscanf(fileStream, "%[$MFF\n]", buff);
		int values[XPM_NUMBER_OF_VALUES];
		for (int i = 0; i < XPM_NUMBER_OF_VALUES; i++)
			fscanf(fileStream, "%d", &values[i]);

		colors = new xpm[values[XPM_COLORS]];
		char** data = new char*[values[XPM_HEIGHT]];
		for (int i = 0; i < values[XPM_HEIGHT]; i++)
			data[i] = new char[values[XPM_WIDTH]];
		this->init(fileName, values[XPM_WIDTH], values[XPM_HEIGHT]);

		for (int i = 0; i < values[XPM_COLORS]; i++)
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
				{
					if (colors[i].id == buff[0])
					{
						for (int n = 0; n < counter; n++)
						{
							img[y][x + n] = i;
						}
						x += counter;
					}
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
		newFileName[strlen(fileName) + strlen(".mff")] = 0;
		this->saveFile(newFileName);
		delete[] newFileName;
	}
}

void file::saveBmpFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "wb");
	int buff;
	int padding = BPM_ROW_ADDRESS_MULTIPLER - ((this->width*BMP_BIT_COUNT / 8) % BPM_ROW_ADDRESS_MULTIPLER);
	if (fileStream != NULL)
	{
		fwrite("BM", sizeof(char), BMP_ELEMENTS_HEADER_START, fileStream);		//Nag³ówek "BM"
		buff = this->width*this->height*BMP_COLORS_PER_PIXEL + BMP_HEADER + padding;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//Ca³kowity rozmiar pliku
		fseek(fileStream, BMP_BYTES_HEADER_TOSKIP, SEEK_CUR);					//Zarezerwowanie bajty
		buff = BMP_OFFSET;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//Pocz¹tek tablicy pixeli
		buff = BMP_SIZE;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//Rozmiar nag³ówka
		fwrite(&(this->width), sizeof(int), BMP_ONE_ELEMENT, fileStream);		//szerokoœæ obrazka
		fwrite(&(this->height), sizeof(int), BMP_ONE_ELEMENT, fileStream);		//wysokoœæ obrazka
		buff = BMP_PLANES;
		fwrite(&buff, sizeof(short int), BMP_ONE_ELEMENT, fileStream);			//iloœæ warstw
		buff = BMP_BIT_COUNT;
		fwrite(&buff, sizeof(short int), BMP_ONE_ELEMENT, fileStream);			//iloœæ bitów na pixel
		buff = BMP_COMPRESION;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//informacja o kompresji
		buff = this->width*this->height*BMP_COLORS_PER_PIXEL + padding;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//rozmiar tablicy pixeli
		buff = BMP_RESOLUTION;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//rozdzielczoœæ obrazka
		buff = BMP_DEFAULT_COLOR_PALETTE;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//paleta kolorów
		buff = BMP_IMPORTANT_COLOR;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//wa¿ne kolory

		buff = 0;
		for (int y = this->height - 1; y >= 0; y--)
		{
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (img[y][x] == i)
					{
						fwrite(&rgbColors[i].b, sizeof(unsigned char), BMP_ONE_ELEMENT, fileStream);
						fwrite(&rgbColors[i].g, sizeof(unsigned char), BMP_ONE_ELEMENT, fileStream);
						fwrite(&rgbColors[i].r, sizeof(unsigned char), BMP_ONE_ELEMENT, fileStream);
					}
			fwrite(&buff, sizeof(unsigned char), padding, fileStream);
		}

		fclose(fileStream);
	}
}

void file::saveXpmFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "w");

	if (fileStream != NULL)
	{
		xpm* colors = new xpm[NUMBER_OF_COLORS];
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
		xpm* colors = new xpm[NUMBER_OF_COLORS];
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
	if (stackCounter > 0)
	{
		delete this->stack[stackCounter - 1];
		this->stackCounter--;
		this->updateImg();
	}
}

void file::addLine()
{
	if (this->interactiveMode != true)
	{
		this->stack[stackCounter] = new line(this->localCursor->getPositionPointer(), this->localCursor->getColorPointer());
		this->stackCounter++;
		interactiveMode = true;

		if (stackCounter == DEFAULT_STACK_SIZE*stackSizeMultipler)
			this->resizeStack();
		drawingMode = drawLine;
	}
}

void file::addRectangle()
{
	if (this->interactiveMode != true)
	{
		this->stack[stackCounter] = new rectangle(this->localCursor->getPositionPointer(), this->localCursor->getColorPointer());
		this->stackCounter++;
		interactiveMode = true;

		if (stackCounter == DEFAULT_STACK_SIZE*stackSizeMultipler)
			this->resizeStack();
		drawingMode = drawRectangle;
	}
}

void file::cancelDrawing()
{
	this->undoLastAction();
	interactiveMode = false;
	drawingMode = none;
}

void file::finishDrawing()
{
	this->stack[stackCounter - 1]->setEnd(this->localCursor->getPositionPointer());
	this->updateImg();
	interactiveMode = false;
	drawingMode = none;
}

bool file::isInteractiveModeEnabled()
{
	return this->interactiveMode;
}

void file::resizeStack()
{
	stackSizeMultipler++;
	shape** temp = new shape*[DEFAULT_STACK_SIZE*stackSizeMultipler];
	for (int i = 0; i < stackCounter; i++)
		temp[i] = stack[i];
	delete[] stack;
	stack = temp;
}

void file::updateView()
{
	for (int y = 0; y < this->height; y++)
		for (int x = 0; x < this->width; x++)
		{
			gotoxy(x + MIN_X_POSITION, y + MIN_Y_POSITION);
			textbackground(img[y][x]);
			putch(' ');
		}
	textcolor(this->localCursor->actualColor);
	textbackground(this->img[this->localCursor->position.y - MIN_Y_POSITION][this->localCursor->position.x - MIN_X_POSITION]);
	if (interactiveMode)
	{
		this->stack[stackCounter - 1]->setEnd(this->localCursor->getPositionPointer());
		this->stack[stackCounter - 1]->setColor(this->localCursor->getColorPointer());
		this->stack[stackCounter - 1]->draw();
	}
	gotoxy(this->localCursor->position.x, this->localCursor->position.y);
	putch(this->localCursor->cursorCharacter);
}

void file::updateImg()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (buff == NULL)
				img[i][j] = WHITE;
			else
				img[i][j] = buff[i][j];
		}
	for (int i = 0; i < stackCounter; i++)
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
{
	return this->drawingMode;
}

const char* file::getFileName()
{
	return this->name;
}

bool file::isUndoEnable()
{
	if (this->stackCounter > 0 && this->interactiveMode != true)
		return true;
	else return false;
}