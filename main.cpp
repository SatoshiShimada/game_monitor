
#include <QtGui>
#include "interface.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Interface *interface = new Interface;
	/* set window size */
	//interface->resize(600, 350);

	interface->show();

	return app.exec();
}

