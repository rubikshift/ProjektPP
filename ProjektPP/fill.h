#include "shape.h"

class fill : public shape
{
	private:
		int colorToChange;
		int maxWidth; 
		int maxHeight;
	public:
	fill(const point* start, const int* colorToSet, const int* colorToChange, int maxWidth, int maxHeight);
	
	void draw(int** img = 0);
};
