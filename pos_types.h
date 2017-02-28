
#ifndef _POS_TYPES_H_
#define _POS_TYPES_H_

#include <cmath>

struct Pos
{
	Pos() : x(0), y(0), th(0) {}
	Pos(float ix, float iy, float ith) :
		x(ix), y(iy), th(ith) {}
	Pos(double ix, double iy, double ith) :
		x((float)ix), y((float)iy), th((float)ith) {}
	float x;
	float y;
	float th;
};

#endif // _POS_TYPES_H_

