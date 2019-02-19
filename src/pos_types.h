#ifndef POS_TYPES_H
#define POS_TYPES_H

enum {
	NONE = 0x00,
	SELF_POS = 0x01,
	BALL = 0x02,
	ENEMY = 0x04,
	GOAL_POLE = 0x08,
};

class Pos
{
public:
	Pos() : x(0.0), y(0.0), th(0.0) {}
	Pos(float ix, float iy, float ith) :
		x(ix), y(iy), th(ith) {}
	Pos(double ix, double iy, double ith) :
		x((float)ix), y((float)iy), th((float)ith) {}
	float x;
	float y;
	float th;
};

class Object
{
public:
	Object(): type(NONE) {}
	Pos pos;
	int type;
};

#endif // POS_TYPES_H

