#include "shape.h"

class fill : public shape
{
	private:
		int colorToChange;
	public:
	fill(const point* start, const int* color);
	
	void draw(int** img = 0);
};
