#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState
{
public:
	GameState();
	~GameState();
	void setData(const char *data, const unsigned int data_len);
	unsigned int getRemainingTime(void);
	bool updatedScore1(void);
	bool updatedScore2(void);
	unsigned int getScore1(void);
	unsigned int getScore2(void);
private:
	void decodeData(const char *data);
	void decodeTeamInfo(const char *data);
	void decodeRobotInfo(const char *data);
	unsigned int remaining_time;
	unsigned int score1;
	unsigned int score2;
	bool f_update_score1;
	bool f_update_score2;
};

#endif // GAME_STATE_H

