
#include "hpl_types.h"

#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QThread>
#include <QString>

/* Constant of infoshare ----------------------- */
static const int NUM_PLAYERS = 6;
static const int COMM_INFO_PORT = 7110;
static const int MAX_COMM_INFO_OBJ = 7;
static const int MAX_STRING = 74;

static const unsigned char COMM_EXIST = 0x80;
static const unsigned char COMM_OUR_SIDE = 0x40;
static const unsigned char COMM_OPPOSITE_SIDE = 0x20;
static const unsigned char COMM_NOT_EXIST = 0x00;

static const int MAX_BLACK_POLES = 6;
static const int MAX_YELLOW_POLES = 1;
static const int MAX_BLUE_POLES = 1;
static const int MAX_MAGENTA_OBJECTS = 3;
static const int MAX_CYAN_OBJECTS = 3;

/* Team color */
static const int MAGENTA = 0;
static const int CYAN    = 1;
/* Constant end --------------------------------- */

struct comm_info_T {
	unsigned char id;
	unsigned char cf_own;
	unsigned char cf_ball;
	unsigned char object[MAX_COMM_INFO_OBJ][4];
	unsigned char status;
	unsigned char fps;
	unsigned char voltage;
	unsigned char temperature;
	unsigned char hishest_servo;
	unsigned char command[MAX_STRING];
};

Q_DECLARE_METATYPE(comm_info_T);

class UdpThread: public QThread {
	Q_OBJECT
public:
	UdpThread(int port_num);
	~UdpThread();
protected:
	struct comm_info_T comm_info;
	int sock;
	void run();
signals:
	void receiveData(struct comm_info_T);
};

bool getCommInfoObject(unsigned char *data, Pos2D *);

