#pragma once
struct point
{
	int x, y;

	bool operator ==(point p);
	bool operator !=(point p);
};
