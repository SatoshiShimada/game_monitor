#ifndef FIELD_SPACE_MANAGER_H
#define FIELD_SPACE_MANAGER_H

#include <vector>

class FieldSpaceManager
{
public:
	FieldSpaceManager(const int, const int);
	~FieldSpaceManager();
	void setObjectPos(const int, const int, const int, const int);
	void clear(void);
	bool getEmptySpace(int &, int &, const int, const int, const int = 0, const int = 0);
private:
	const int EXIST;
	const int EMPTY;
	const int grid_num_x;
	const int grid_num_y;
	std::vector<std::vector<int>> grid_map;
	const int field_width;
	const int field_height;
};

#endif // FIELD_SPACE_MANAGER_H

