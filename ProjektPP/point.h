#pragma once
struct point
{
	int x, y;

	bool operator ==(point p);
	void operator =(point p);
};
