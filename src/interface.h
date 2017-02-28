
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <iostream>
#include <vector>

#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QMainWindow>
#include <QDropEvent>
#include <QDrag>
#include <QUrl>
#include <QMimeData>
#include <QString>
#include <QMetaType>
#include <QPainter>
#include <QSettings>

#include "udp_thread.h"
#include "log.h"
#include "pos_types.h"

struct PositionMaker {
	PositionMaker() : enable_pos(false), enable_ball(false), lastReceive(0) { color[0] = '\0'; }
	bool enable_pos;
	bool enable_ball;
	int lastReceive;
	char color[20];
	Pos pos; /* self position */
	Pos ball; /* ball position */
};

struct robot {
	QLabel *name;
	QLabel *voltage;
	QLabel *fps;
	QLabel *string;
	QLabel *cf_own;
	QLabel *cf_ball;
};

class Interface : public QMainWindow
{
	Q_OBJECT

private:
	Log log;
	std::vector<UdpThread *> th;
	QCheckBox *receive, *reverse;
	QSettings *settings;
	QLabel *id, *name, *voltage, *fps, *string, *cf_own, *cf_ball;
	QString filenameDrag;
	QWidget *window;
	std::vector<PositionMaker> positions;
	std::vector<struct robot> robot;
	std::vector<QLabel *> idLabel;
	std::vector<QWidget *>robotState;
	std::vector<QGridLayout *>idLayout;
	QLabel *image;
	QPixmap map;
	QPixmap origin_map;
	QGridLayout *mainLayout;
	QHBoxLayout *checkLayout;
	QGridLayout *labelLayout;
	QPalette pal_state_bgcolor;
	QPalette pal_red;
	QPalette pal_green;
	QPalette pal_blue;
	QPalette pal_black;
	QPalette pal_orange;
	void initializeConfig(void);
	void createWindow(void);
	void connection(void);
	bool fLogging;
	bool fReceive;
	bool fReverse;
	const int robot_num;

protected:
	void paintEvent(QPaintEvent *);

public:
	Interface();
	~Interface();
	void loadImage(const char *);
	void loadImage(QString);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
	void decodeUdp(struct comm_info_T, struct robot *, int num);

private slots:
	void decodeData1(struct comm_info_T);
	void decodeData2(struct comm_info_T);
	void decodeData3(struct comm_info_T);
	void decodeData4(struct comm_info_T);
	void decodeData5(struct comm_info_T);
	void decodeData6(struct comm_info_T);
	void receiveStateChange(int state);
	void reverseField(int state);
};

#endif // _INTERFACE_H_

