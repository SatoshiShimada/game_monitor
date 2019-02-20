#include <utility>
#include <cmath>

#include "field_space_manager.h"

static inline int distance(const int x1, const int y1, const int x2, const int y2)
{
	const int x = x1 - x2;
	const int y = y1 - y2;
	return std::sqrt(x * x + y * y);
}

FieldSpaceManager::FieldSpaceManager(const int field_w, const int field_h) : grid_num_x(20), grid_num_y(20), grid_map(grid_num_y, std::vector<int>(grid_num_x, EMPTY)), field_width(field_w), field_height(field_h)
{
}

FieldSpaceManager::~FieldSpaceManager()
{
}

void FieldSpaceManager::setObjectPos(const int x, const int y, const int w, const int h)
{
	const int step_x = field_width / grid_num_x;
	const int step_y = field_height / grid_num_y;
	for(int yi = (y - h / 2) / step_y; yi * step_y < (y + h / 2); yi++) {
		for(int xi = (x - w / 2) / step_x; xi * step_x < (x + w / 2); xi++) {
			if(yi < 0 || yi >= grid_num_y) continue;
			if(xi < 0 || xi >= grid_num_x) continue;
			grid_map[yi][xi] = EXIST;
		}
	}
}

void FieldSpaceManager::clear(void)
{
	for(int i = 0; i < grid_num_y; i++) {
		for(int j = 0; j < grid_num_x; j++) {
			grid_map[i][j] = EMPTY;
		}
	}
}

bool FieldSpaceManager::getEmptySpace(int &space_x, int &space_y, const int request_w, const int request_h, const int priority_x, const int priority_y)
{
	const int grid_step_x = field_width / grid_num_x;
	const int grid_step_y = field_height / grid_num_y;
	const int necessary_grid_num_x = request_w / grid_step_x + 1;
	const int necessary_grid_num_y = request_h / grid_step_y + 1;
	int start_x, start_y;
	std::vector<std::pair<int, int>> spaces;
	for(int yi = 0; yi < grid_num_y; yi++) {
		for(int xi = 0; xi < grid_num_x; xi++) {
			if(grid_map[yi][xi] == EMPTY) {
				start_x = xi;
				start_y = yi;
				for(int yyi = 0; yyi < necessary_grid_num_y; yyi++) {
					for(int xxi = 0; xxi < necessary_grid_num_x; xxi++) {
						if(yi + yyi >= grid_num_y) goto exit_loop;
						if(xi + xxi >= grid_num_x) goto exit_loop;
						if(grid_map[yi + yyi][xi + xxi] != EMPTY) {
							goto exit_loop;
						}
					}
				}
				spaces.push_back(std::pair<int, int>(start_x, start_y));
			}
exit_loop: ;
		}
	}
	if(spaces.size() > 0) {
		int min_index = 0;
		int min_distance = 10000;
		for(int index = 0; index < spaces.size(); index++) {
			const int posx = grid_step_x * spaces[index].first + static_cast<int>(necessary_grid_num_x / 2.0 * grid_step_x);
			const int posy = grid_step_y * spaces[index].second + static_cast<int>(necessary_grid_num_y / 2.0 * grid_step_y);
			const int dist = distance(posx, posy, priority_x, priority_y);
			if(dist < min_distance) {
				min_distance = dist;
				min_index = index;
			}
		}
		const int xi = spaces[min_index].first;
		const int yi = spaces[min_index].second;
		for(int yyi = 0; yyi < necessary_grid_num_y; yyi++) {
			for(int xxi = 0; xxi < necessary_grid_num_x; xxi++) {
				if(yi + yyi >= grid_num_y) continue;
				if(xi + xxi >= grid_num_x) continue;
				grid_map[yi + yyi][xi + xxi] = EXIST;
			}
		}
		// return center pos of space
		space_x = grid_step_x * xi + static_cast<int>(necessary_grid_num_x / 2.0 * grid_step_x);
		space_y = grid_step_y * yi + static_cast<int>(necessary_grid_num_y / 2.0 * grid_step_y);
		return true;
	}
	// no space found
	space_x = 0;
	space_y = 0;
	return false;
}

