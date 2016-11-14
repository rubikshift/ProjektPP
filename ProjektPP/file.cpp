#include <string.h>
#include <stdio.h>
#include "conio2.h"
#include "file.h"
#include "projektPP.h"
#include "rgb.h"
#include "xpm.h"

typedef FILE onDiskFile;

file::file(const char* fileName)
{
	this->loadFile(fileName);
}

file::file(const char* name, int width, int height)
{
	this->init(name, width, height);
	buff = NULL;
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
	stack = new shape*[DEFAULT_STACK_SIZE];
	stackSizeMultipler = 1;
	stackCounter = 0;
	this->height = height;
	this->width = width;
	localCursor = new cursor(this->width, this->height);
	interactiveMode = false;
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
	buff = new int*[this->height];
	for (int y = 0; y < this->height; y++)
	{
		buff[y] = new int[this->width];
		for (int x = 0; x < this->width; x++)
			buff[y][x] = img[y][x];
	}
}

void file::loadBmpFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "rb");
	unsigned char info[BMP_HEADER];
	if (fileStream != NULL)
	{
		fread(info, sizeof(unsigned char), BMP_HEADER, fileStream);

		this->init(fileName, *(int*)&info[BMP_WIDTH_INFO], *(int*)&info[BMP_HEIGHT_INFO]);

		unsigned char** data = new unsigned char*[this->height];
		for (int i = 0; i < height; i++)
		{
			data[i] = new unsigned char[BMP_COLORS_PER_PIXEL * this->width];
			fread(data[i], sizeof(unsigned char), BMP_COLORS_PER_PIXEL * this->width, fileStream);
		}
		fclose(fileStream);

		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (data[y][x * BMP_COLORS_PER_PIXEL + BMP_R_POSITION] == rgbColors[i].r
						&& data[y][x * BMP_COLORS_PER_PIXEL + BMP_G_POSITION] == rgbColors[i].g
						&& data[y][x * BMP_COLORS_PER_PIXEL + BMP_B_POSITION] == rgbColors[i].b)
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
				if (strstr(buff, hexColor[j]) != NULL || strstr(buff, hexColor[j+NUMBER_OF_COLORS]) != NULL)
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
		delete[] data;
	}
}

void file::loadMffFile(const char* fileName)
{

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
	case undefined: break;
	}
}

void file::saveBmpFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "wb");	
	int buff;
	if (fileStream != NULL)
	{
		fwrite("BM", sizeof(char), BMP_ELEMENTS_HEADER_START, fileStream);		//Nag³ówek "BM"
		buff = this->width*this->height*BMP_COLORS_PER_PIXEL + BMP_HEADER;
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
		buff = this->width*this->height*BMP_COLORS_PER_PIXEL;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//rozmiar tablicy pixeli
		buff = BMP_RESOLUTION;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//rozdzielczoœæ obrazka
		buff = BMP_DEFAULT_COLOR_PALETTE;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//paleta kolorów
		buff = BMP_IMPORTANT_COLOR;
		fwrite(&buff, sizeof(int), BMP_ONE_ELEMENT, fileStream);				//wa¿ne kolory

		for (int y = 0; y < this->height; y++)
			for (int x = 0; x < this->width; x++)
				for (int i = 0; i < NUMBER_OF_COLORS; i++)
					if (img[y][x] == i)
					{
						fwrite(&rgbColors[i].b, sizeof(unsigned char), BMP_ONE_ELEMENT, fileStream);
						fwrite(&rgbColors[i].g, sizeof(unsigned char), BMP_ONE_ELEMENT, fileStream);
						fwrite(&rgbColors[i].r, sizeof(unsigned char), BMP_ONE_ELEMENT, fileStream);
					}
		fclose(fileStream);
	}
}

void file::saveXpmFile(const char* fileName)
{
	onDiskFile* fileStream = fopen(fileName, "w");
	
	if(fileStream != NULL)
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
					if (img[y][x] == colors[i].color && x < this->width - 1)
						fprintf(fileStream, "%c", colors[i].id);
					else if (img[y][x] == colors[i].color && x == this->width - 1)
						fprintf(fileStream, "%c\n", colors[i].id);
				}
		fclose(fileStream);
		delete[] colors;
	}
}

void file::saveMffFile(const char* fileName)
{

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
	}
}

void file::cancelDrawing()
{
	this->undoLastAction();
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
	for (int y = 0; y < this->height; y++)
		for (int x = 0; x < this->width; x++)
		{
			gotoxy(x + MIN_X_POSITION, y + MIN_Y_POSITION);
			textbackground(img[y][x]);
			putch(' ');
		}
	gotoxy(this->localCursor->position.x, this->localCursor->position.y);
	textcolor(this->localCursor->actualColor);
	textbackground(this->img[this->localCursor->position.y - MIN_Y_POSITION][this->localCursor->position.x - MIN_X_POSITION]);
	putch(this->localCursor->cursorCharacter);
	if (interactiveMode)
	{
		this->stack[stackCounter - 1]->setEnd(this->localCursor->getPositionPointer());
		this->stack[stackCounter - 1]->draw();
	}
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