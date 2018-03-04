#ifndef _LOG_H_
#define _LOG_H_

class LogWriter {
public:
	LogWriter();
	~LogWriter();
	int write(int, char *, int, double, int, int, float, int, int, int, int, int, int, char *, int, int);
	int separate(void);
	void setEnable(bool = true);
private:
	void openFile(char *);
	void closeFile(void);
	FILE *fp;
	bool opened;
	bool enable;
};

#endif // _LOG_H_

