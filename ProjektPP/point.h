#pragma once
struct point
{
	int x, y;

	bool operator ==(point p);
	point operator =(point p);
};
