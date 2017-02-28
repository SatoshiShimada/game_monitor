
#include <QtGui>

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>

#include "interface.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Interface *interface = new Interface;

	interface->show();

	return app.exec();
}

