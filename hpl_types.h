
#ifndef _HPL_TYPES_H_
#define _HPL_TYPES_H_

#include <cmath>

struct Pos2D
{
	Pos2D() : x(0), y(0), th(0) {}
	Pos2D(float ix, float iy, float ith) :
		x(ix), y(iy), th(ith) {}
	Pos2D(double ix, double iy, double ith) :
		x((float)ix), y((float)iy), th((float)ith) {}
	float x;
	float y;
	float th;
	Pos2D GlobalToLocal(const Pos2D &self_pos) const {
		float dx = x - self_pos.x;
		float dy = y - self_pos.y;
		double r = sqrt(dx * dx + dy * dy);
		double phi = atan2(dy, dx);
		Pos2D fs;
		fs.x = (float)(r * cos(phi - self_pos.th));
		fs.y = (float)(r * sin(phi - self_pos.th));
		fs.th = th - self_pos.th;
		return fs;
	}
	Pos2D LocalToGlobal(const Pos2D &self_pos) const {
		float r = (float)sqrt(x * x + y * y);
		float phi = (float)atan2(y, x);
		Pos2D gl;
		gl.x = (float)(self_pos.x + r * cos(self_pos.th + phi));
		gl.y = (float)(self_pos.y + r * sin(self_pos.th + phi));
		gl.th = self_pos.th + th;
		return gl;
	}
	float distanceTo(const Pos2D &pos) const {
		return sqrt((pos.x - x) * (pos.x - x) + (pos.y - y) * (pos.y - y));
	}
};

#endif // _HPL_TYPES_H_

