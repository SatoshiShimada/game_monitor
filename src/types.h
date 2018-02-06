#ifndef _TYPES_H_
#define _TYPES_H_

#include <string>
#include <time.h>
#include <cmath>

static const int NUM_PLAYERS = 6;
static const int COMM_INFO_PORT = 7110;
static const int MAX_COMM_INFO_OBJ = 7;
static const int MAX_STRING = 74;

static const unsigned char COMM_EXIST = 0x80;
static const unsigned char COMM_OUR_SIDE = 0x40;
static const unsigned char COMM_OPPOSITE_SIDE = 0x20;
static const unsigned char COMM_NOT_EXIST = 0x00;

static const int MAX_BLACK_POLES = 6;
static const int MAX_YELLOW_POLES = 1;
static const int MAX_BLUE_POLES = 1;
static const int MAX_MAGENTA_OBJECTS = 3;
static const int MAX_CYAN_OBJECTS = 3;

static const int MAGENTA = 0;
static const int CYAN    = 1;

struct comm_info_T {
	unsigned char id;
	unsigned char cf_own;
	unsigned char cf_ball;
	unsigned char object[MAX_COMM_INFO_OBJ][4];
	unsigned char status;
	unsigned char fps;
	unsigned char voltage;
	unsigned char temperature;
	unsigned char highest_servo;
	unsigned char command[MAX_STRING];
};

enum {
	NONE = 0x00,
	SELF_POS = 0x01,
	BALL = 0x02,
	ENEMY = 0x04,
	GOAL_POLE = 0x08,
};

inline static double radian(const double degree)
{
	return degree * M_PI / 180.0;
}

class Pos
{
public:
	Pos() : x(0), y(0), th(0) {}
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

class GameState {
};

class Role {
public:
	Role()
	{
	}
	virtual ~Role()
	{
	}
	virtual char getColor(void) = 0;
};

class RoleNeutral : public Role {
public:
	RoleNeutral()
	{
	}
	~RoleNeutral()
	{
	}
	char getColor(void)
	{
		return 'r';
	}
};

class RobotState {
public:
	RobotState() : role(NULL) {}
	~RobotState()
	{
		delete role;
	}
	void setData(struct comm_info_T comm_info)
	{
		/* color; MAGENTA or CYAN */
		color = (int)(comm_info.id & 0x80) >> 7;
		id    = (int)(comm_info.id & 0x7F);
		/* voltage */
		voltage = (comm_info.voltage << 3) / 100.0;
		/* fps */
		fps = (int)comm_info.fps;
		/* highest servo id */
		highest_servo = (int)comm_info.highest_servo;
		/* highest servo temperature */
		temperature = (int)comm_info.temperature;

		/* record time of receive data */
		time_t timer;
		struct tm *local_time;
		timer = time(NULL);
		local_time = localtime(&timer);
		receive_time = *local_time;

		/* self-position confidence */
		cf_selfpos = comm_info.cf_own;
		/* ball position confidence */
		cf_ball = comm_info.cf_ball;
		/* role and message */
		std::string command_string((char *)comm_info.command);
		if(strstr((const char *)comm_info.command, "Attacker")) {
			role = (Role*)new RoleNeutral;
		} else if(strstr((const char *)comm_info.command, "Neutral")) {
			role = (Role*)new RoleNeutral;
		} else if(strstr((const char *)comm_info.command, "Defender")) {
			role = (Role*)new RoleNeutral;
		} else if(strstr((const char *)comm_info.command, "Keeper")) {
			role = (Role*)new RoleNeutral;
		} else {
			role = (Role*)new RoleNeutral;
		}

		for(int i = 0; i < MAX_COMM_INFO_OBJ; i++) {
			Object obj;
			bool exist = getCommInfoObject(comm_info.object[i], obj);
			if(!exist) continue;
			if(obj.type == NONE) continue;
			objects.push_back(obj);
		}
	}
private:
	bool getCommInfoObject(unsigned char *data, Object &obj)
	{
		bool data_exist = false;

		/* calculate position of object */
		if(data[0] & COMM_EXIST) {
			data_exist = true;
			int x = ((data[0] & 0x0f) << 6) + ((data[1] & 0xfc) >> 2);
			if((x & 0x0200) != 0) x = -(0x0400 - x);
			x *= 10;
			int y = ((data[1] & 0x03) << 8) + data[2];
			if((y & 0x0200) != 0) y = -(0x0400 - y);
			y *= 10;
			const int theta = (data[3] * 2) - 180;
			const float th = (float)radian(theta);
			bool is_our_side = ((data[0] & COMM_OUR_SIDE) != 0) ? true : false;
			bool is_opposite_side = ((data[0] & COMM_OPPOSITE_SIDE) != 0) ? true : false;
			if(is_our_side && is_opposite_side) obj.type = BALL;
			if(is_our_side && !is_opposite_side) obj.type = SELF_POS;
			if(!is_our_side && is_opposite_side) obj.type = ENEMY;
			if(!is_our_side && !is_opposite_side) obj.type = GOAL_POLE;
			obj.pos.x = x;
			obj.pos.y = y;
			obj.pos.th = th;
		}
		return data_exist;
	}
	int id;
	int color;
	int fps;
	double voltage;
	double temperature;
	int highest_servo;
	std::string command;
	double cf_selfpos;
	double cf_ball;
	std::vector<Object> objects;
	struct tm receive_time;
	Role *role;
};

#endif // _TYPES_H_

