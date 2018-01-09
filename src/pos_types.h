
#ifndef _POS_TYPES_H_
#define _POS_TYPES_H_

enum {
	NONE = 0x00,
	SELF_POS = 0x01,
	BALL = 0x02,
	ENEMY = 0x04,
	GOAL_POLE = 0x08,
};

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

struct Object
{
	Object(): type(NONE) {}
	struct Pos pos;
	int type;
};

#endif // _POS_TYPES_H_

