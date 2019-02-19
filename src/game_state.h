#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState
{
public:
	GameState();
	~GameState();
	void setData(const char *data, const int data_len);
	int getRemainingTime(void);
	bool updatedScore1(void);
	bool updatedScore2(void);
	int getScore1(void);
	int getScore2(void);
private:
	void decodeData(const char *data);
	void decodeTeamInfo(const char *data);
	void decodeRobotInfo(const char *data);
	int remaining_time;
	int score1;
	int score2;
	bool f_update_score1;
	bool f_update_score2;
};

#endif // GAME_STATE_H

