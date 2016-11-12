#pragma once
#include "point.h"
#include "cursor.h"
#include "shape.h"
#include "line.h"
#include "rectangle.h"
#include "extensions.h"
#include "updateModes.h"

class file
{
	private:
		char* name;
		int width;
		int height;
		short int** img;
		shape** stack;
		int stackCounter;
		int stackSizeMultipler;
		bool interactiveMode;

		extension getFileExtension(const char* fileName);
		void loadBmpFile(const char* fileName);
		void loadXpmFile(const char* fileName);
		void loadMffFile(const char* fileName);

		void init(const char* name, int width, int height);
	public:
		cursor* localCursor;
		
		file(const char* name, int width, int height);
		~file();
		
		void loadFile(const char* fileName);
		void saveFile(const char* fileName, extension fileExtension);

		void undoLastAction(updateMode mode = all);
		void addLine();
		void addRectangle();
		void cancelDrawing();
		void finishDrawing();
		bool isInteractiveModeEnabled();
		
		void resizeStack();

		void updateView();
		void updateImg(updateMode mode = last);
};
