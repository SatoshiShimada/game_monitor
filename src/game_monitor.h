#ifndef _GAME_MONITOR_H_
#define _GAME_MONITOR_H_

#include <vector>
#include "<types.h"

class GameMonitor {
public:
	GameMonitor() {}
	~GameMonitor() {}
public slots:
	void setRobotState(int);
	void SetGameState(void);
signals:
	void UpdateRobotState(void);
	void UpdateGameState(void);
private:
	std::vector<RobotState> robot_state;
	GameState game_state;
	std::vector<UdpServer *> udp_thread;
};

#endif // _GAME_MONITOR_H_

GameMonitor::GameMonitor()
{
	robot_state(6);
	for(int i = 0; i < 6; i++) {
		udp_thread.push_back(new UdpServer(id));
		connect(*udp_thread[i], SIGNAL(received()), robot_state[i], SLOT(setData()));
		connect(robot_state[i], SIGNAL(updateState(int)), this, SLOT(setRobotState(int)));
	}
	connect(game_state, SIGNAL(updateState()), this, SLOT(setGameState()));
}

void GameMonitor::setRobotState(int id)
{
	emit(updateRobotState());
}

