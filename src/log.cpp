
#include <iostream>
#include <stdio.h>
#include <time.h>

#include "log.h"

Log::Log()
{
	time_t timer;
	struct tm *local_time;
	char filename[1024];
	char logfile_path[] = "log/";

	timer = time(NULL);
	local_time = localtime(&timer);
	sprintf(filename, "%s%d-%d-%d-%d-%d.log", logfile_path, local_time->tm_year+1900, local_time->tm_mon+1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min);
	failed = false;
	enable = true;
	fp = fopen(filename, "w");
	if(!fp) {
		failed = true;
		enable = false;
	}
}

int Log::write(int id, char *color, int fps, double voltage,
	int posx, int posy, float posth, int ballx, int bally, char *str)
{
	time_t timer;
	struct tm *local_time;

	timer = time(NULL);
	local_time = localtime(&timer);
	if(!failed && enable) {
		fprintf(fp, "%d:%d:%d,", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
		fprintf(fp, "%d,", id);
		fprintf(fp, "%s,", color);
		fprintf(fp, "%d,", fps);
		fprintf(fp, "%.2lf,", voltage);
		fprintf(fp, "%d,%d,%f,", posx, posy, posth);
		fprintf(fp, "%d,%d,", ballx, bally);
		fprintf(fp, "%s", str);
		fprintf(fp, "\n");
	}
	return 0;
}

int Log::separate(void)
{
	if(!failed && enable) {
		fprintf(fp, "\n---\n");
	}
	return 0;
}

void Log::setEnable()
{
	enable = true;
}

void Log::setDisable()
{
	enable = false;
}

Log::~Log()
{
	if(!failed) {
		fclose(fp);
	}
}

