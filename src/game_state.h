#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState
{
public:
	GameState();
	~GameState();
	void setData(const char *data, const int data_len);
	int getRemainingTime(void);
private:
	void decodeData(const char *data);
	void decodeTeamInfo(const char *data);
	void decodeRobotInfo(const char *data);
	int remaining_time;
};

#endif // GAME_STATE_H

