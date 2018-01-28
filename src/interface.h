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
#include <QSlider>
#include <QMetaType>
#include <QPainter>
#include <QSettings>
#include <QGridLayout>
#include <QProgressBar>
#include <QFileDialog>

#include "udp_thread.h"
#include "log_writer.h"
#include "pos_types.h"

class PositionMarker {
public:
	PositionMarker() : enable_pos(false), enable_ball(false), enable_goal_pole{false, false} { color[0] = '\0'; }
	bool enable_pos;
	bool enable_ball;
	bool enable_goal_pole[2];
	struct tm lastReceiveTime;
	char color[20];
	Pos pos; /* self position */
	Pos ball; /* ball position */
	Pos goal_pole[2]; /* goal pole position */
};

class LogData {
public:
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
	int goal_pole_x1;
	int goal_pole_y1;
	int goal_pole_x2;
	int goal_pole_y2;
	int cf_own;
	int cf_ball;
	char msg[100];
};

class ClickWidget : public QWidget
{
	Q_OBJECT
private:
	void mouseReleaseEvent(QMouseEvent *event) override
	{
		if(event->button() == Qt::LeftButton) {
			emit clicked();
		}
	}
signals:
	void clicked(void);
};

class Interface : public QMainWindow
{
	Q_OBJECT

private:
	LogWriter log_writer;
	std::vector<UdpServer *> th;
	QCheckBox *reverse;
	QPushButton *loadLogButton;
	QPushButton *log1Button, *log2Button, *log5Button;
	QSettings *settings;
	QString filenameDrag;
	QWidget *window;
	QLabel *image;
	QLabel *log_step;
	QPixmap map;
	QPixmap origin_map;
	QPixmap team_logo_map;
	QSlider *log_slider;
	QGridLayout *mainLayout;
	QHBoxLayout *checkLayout;
	QGridLayout *labelLayout;
	QHBoxLayout *logLayout;
	QHBoxLayout *logSpeedButtonLayout;
	QPalette pal_state_bgcolor;
	QPalette pal_red;
	QPalette pal_green;
	QPalette pal_blue;
	QPalette pal_black;
	QPalette pal_orange;
	std::vector<PositionMarker> positions;
	std::vector<RobotStatus> robot;
	std::vector<QLabel *> idLabel;
	std::vector<ClickWidget *>robotState;
	std::vector<QGridLayout *>idLayout;
	std::vector<LogData> log_data;
	bool fLogging;
	bool fReverse;
	int updateMapTimerId;
	unsigned int log_count;
	const int max_robot_num;
	int logo_pos_x, logo_pos_y;
	int log_speed;
	int select_robot_num;
	struct tm last_select_time;
	void initializeConfig(void);
	void createWindow(void);
	void connection(void);
	int getInterval(QString, QString);
	Pos globalPosToImagePos(Pos);
	void timerEvent(QTimerEvent *);
	void setParamFromFile(std::vector<std::string>);
	void setData(LogData);
	QColor getColor(const char *);
	void selectRobot(int);

public:
	Interface();
	~Interface();
	void loadImage(const char *);
	void loadImage(QString, QString);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
	void decodeUdp(struct comm_info_T, RobotStatus *, int num);
	void updateMap(void);

private slots:
	void decodeData(int, struct comm_info_T);
	void selectRobot1(void);
	void selectRobot2(void);
	void selectRobot3(void);
	void selectRobot4(void);
	void selectRobot5(void);
	void selectRobot6(void);
	void reverseField(int state);
	void loadLogFile(void);
	void updateLog(void);
	void logSpeed1X(void);
	void logSpeed2X(void);
	void logSpeed5X(void);
};

#endif // _INTERFACE_H_

