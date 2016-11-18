#pragma once
#include "mode.h"
#include "point.h"
#include "cursor.h"
#include "shape.h"
#include "extensions.h"

class file
{
	private:
		char* name;
		int width;
		int height;
		int** img;
		int** buff;
		shape** stack;
		int stackCounter;
		int stackSizeMultipler;
		bool interactiveMode;
		mode drawingMode;

		extension getFileExtension(const char* fileName);
		
		void loadBmpFile(const char* fileName);
		void loadXpmFile(const char* fileName);
		void loadMffFile(const char* fileName);

		void saveBmpFile(const char* fileName);
		void saveXpmFile(const char* fileName);
		void saveMffFile(const char* fileName);

		void init(const char* name, int width, int height);
	public:
		cursor* localCursor;
		
		file(const char* fileName);
		file(const char* name, int width, int height);
		~file();
		
		void loadFile(const char* fileName);
		void saveFile(const char* fileName = NULL);

		void undoLastAction();
		void addLine();
		void addRectangle();
		void cancelDrawing();
		void finishDrawing();

		bool isInteractiveModeEnabled();
		bool isUndoEnable();
		mode getDrawingMode();

		void resizeStack();

		void updateView();
		void updateImg();

		const char* getFileName();
};
