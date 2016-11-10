#pragma once
#include "point.h"
#include "cursor.h"
#include "shape.h"

class file
{
	private:
		char* name;
		int** buff;
		shape** stack;
		int stackCounter;
		int stackSizeMultipler;
		bool interactiveMode;

	public:
		cursor localCursor;
		
		file(const char* name, int width, int height);
		~file();
		
		void loadFile(const char* fileName);
		void saveFile(const char* fileName);

		void undoLastAction();
		void addLine();
		void addRectangle();
		void cancelDrawing();
		void finishDrawing();
		bool isInteractiveModeEnabled();
		
		void resizeStack();

		void updateView();
		void updateBuff();
};
