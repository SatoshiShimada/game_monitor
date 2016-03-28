
#include <stdio.h>

class Log {
public:
	Log();
	~Log();
	int write(const char *);
	int separate(void);
private:
	FILE *fp;
	bool failed;
};

