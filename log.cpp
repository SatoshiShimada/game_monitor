
#include <iostream>
#include <stdio.h>

#include "log.h"

Log::Log()
{
	failed = false;
	fp = fopen("log.txt", "a");
	if(!fp) {
		failed = true;
	}
}

int Log::write(const char *str)
{
	if(!failed) {
		fprintf(fp, "%s\n", str);
	}
	return 0;
}

int Log::separate(void)
{
	if(!failed) {
		fprintf(fp, "\n---\n");
	}
	return 0;
}

Log::~Log()
{
	if(!failed) {
		fclose(fp);
	}
}

