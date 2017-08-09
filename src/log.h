
#ifndef _LOG_H_
#define _LOG_H_

class Log {
public:
	Log();
	~Log();
	int write(int, char *, int, double, int, int, float, int, int, char *, int, int);
	int separate(void);
	void setEnable(void);
	void setDisable(void);
private:
	FILE *fp;
	bool failed;
	bool enable;
};

#endif // _LOG_H_

