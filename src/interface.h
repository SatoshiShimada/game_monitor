
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <vector>
#include <string>

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

struct PositionMarker {
	PositionMarker() : enable_pos(false), enable_ball(false) { color[0] = '\0'; }
	bool enable_pos;
	bool enable_ball;
	struct tm lastReceiveTime;
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
	QProgressBar *cf_own_bar;
	QProgressBar *cf_ball_bar;
};

struct log_data_t {
	char time_str[100];
	int id;
	char color_str[100];
	int fps;
	double voltage;
	int x;
	int y;
	double theta;
	int ball_x;
	int ball_y;
	char msg[100];
};

class Interface : public QMainWindow
{
	Q_OBJECT

private:
	Log log;
	std::vector<UdpServer *> th;
	QCheckBox *reverse;
	QPushButton *loadLogButton;
	QSettings *settings;
	QLabel *id, *name, *voltage, *fps, *string, *cf_own, *cf_ball;
	QString filenameDrag;
	QWidget *window;
	std::vector<PositionMarker> positions;
	std::vector<struct robot> robot;
	std::vector<QLabel *> idLabel;
	std::vector<QWidget *>robotState;
	std::vector<QGridLayout *>idLayout;
	QLabel *image;
	QPixmap map;
	QPixmap origin_map;
	QPixmap team_logo_map;
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
	int getInterval(QString, QString);
	Pos globalPosToImagePos(Pos);
	std::vector<struct log_data_t> log_data;
	bool fLogging;
	bool fReverse;
	int updateMapTimerId;
	unsigned int log_count;
	const int max_robot_num;
	int logo_pos_x, logo_pos_y;

protected:
	void timerEvent(QTimerEvent *);
	void setParamFromFile(std::vector<std::string>);
	void setData(struct log_data_t);

public:
	Interface();
	~Interface();
	void loadImage(const char *);
	void loadImage(QString, QString);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
	void decodeUdp(struct comm_info_T, struct robot *, int num);
	void updateMap(void);

private slots:
	void decodeData1(struct comm_info_T);
	void decodeData2(struct comm_info_T);
	void decodeData3(struct comm_info_T);
	void decodeData4(struct comm_info_T);
	void decodeData5(struct comm_info_T);
	void decodeData6(struct comm_info_T);
	void reverseField(int state);
	void loadLogFile(void);
	void updateLog(void);
};

#endif // _INTERFACE_H_

