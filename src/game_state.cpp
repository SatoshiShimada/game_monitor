#include "game_state.h"

GameState::GameState() : remaining_time(0), score1(0), score2(0), f_update_score1(false), f_update_score2(false)
{
}

GameState::~GameState()
{
}

void GameState::setData(const char *data, const int data_len)
{
	constexpr int packet_size = 640;
	if(data_len != packet_size)
		return;
	decodeData(data + 0);
	decodeTeamInfo(data + 24);
	decodeTeamInfo(data + 24 + 308);
}

void GameState::decodeData(const char *data)
{
	if(!(data[0] == 'R' && data[1] == 'G' && data[2] == 'm' && data[3] == 'e')) {
		return;
	}
	const int protcol_version = data[4] << 8 | data[5];
	const int packet_number = data[6];
	const int players_per_team = data[7];
	const int game_type = data[8];
	const int state = data[9];
	const int first_half = data[10];
	const int kick_off_team = data[11];
	const int secondary_state = data[12];
	const int secondary_state_info = data[13] << 3 | data[14] << 2 | data[15] << 1 | data[16];
	const int drop_in_team = data[17];
	const int drop_in_time = data[18] << 8 | data[19];
	const int secs_remaining = data[20] << 8 | data[21];
	const int secondary_time = data[22] << 8 | data[23];

	constexpr int PROTCOL_VERSION = 12;
	if(protcol_version != PROTCOL_VERSION)
		return;
	remaining_time = secs_remaining;
}

void GameState::decodeTeamInfo(const char *data)
{
	const int team_number = data[0];
	const int team_color = data[1];
	const int score = data[2];
	const int penalty_shot = data[3];
	const int single_shots = data[4] << 8 | data[5];
	const int coach_sequence = data[6];
	const char *coach_message = data + 7;
	constexpr int coach_message_size = 253;
	decodeRobotInfo(data + 7 + coach_message_size);
	constexpr int max_players = 11;
	for(int i = 0; i < max_players; i++) {
		constexpr int sizeof_robot_info = 4;
		decodeRobotInfo(data + 260 + sizeof_robot_info + (sizeof_robot_info * i));
	}

	constexpr int TEAM_COLOR_BLUE = 0;
	constexpr int TEAM_COLOR_RED = 1;

	if(team_color == TEAM_COLOR_BLUE && score1 != score) {
		f_update_score1 = true;
		score1 = score;
	} else if(team_color == TEAM_COLOR_RED && score2 != score) {
		f_update_score2 = true;
		score2 = score;
	}
}

void GameState::decodeRobotInfo(const char *data)
{
	const int penalty = data[0];
	const int secs_till_unpenalised = data[1];
	const int yello_card_count = data[2];
	const int red_card_count = data[3];
}

int GameState::getRemainingTime(void)
{
	return remaining_time;
}

bool GameState::updatedScore1(void)
{
	return f_update_score1;
}

bool GameState::updatedScore2(void)
{
	return f_update_score2;
}

int GameState::getScore1(void)
{
	f_update_score1 = false;
	return score1;
}

int GameState::getScore2(void)
{
	f_update_score2 = false;
	return score2;
}

