#pragma once
#include "point.h"
#include "cursor.h"
#include "shape.h"

class file
{
	private:
		char* name;
		int** buff;
		shape* stack;
		int stackCounter;
		bool interactiveMode;

		void toggleInteractiveMode();
	public:
		cursor localCursor;

		void createNewFile(int width, int height);
		void loadFile(const char* fileName);
		void saveFile(const char* fileName);

		void undoLastAction();
		void addLine(const point* start);
		void addRectangle(const point* start);
		bool isInteractiveModeEnabled();

		void updateView();
		void updateBuff();
};