#ifndef LOG_H
#define LOG_H

class LogWriter {
public:
	LogWriter();
	~LogWriter();
	int startRecord(const char *);
	int stopRecord(void);
	int write(int, const char *, int, double, int, int, float, int, int, int, int, int, int, const char *, int, int);
	void writeScore(const int, const int);
	void writeTime(const int);
	int separate(void);
	void setEnable(bool = true);
private:
	void openFileCurrentTime(void);
	void openFile(char *);
	void closeFile(void);
	FILE *fp;
	bool opened;
	bool enable;
};

#endif // LOG_H

