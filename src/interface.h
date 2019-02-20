#ifndef INTERFACE_H
#define INTERFACE_H

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
#include <QPixmap>

#include "udp_thread.h"
#include "log_writer.h"
#include "pos_types.h"
#include "capture.h"
#include "aspect_ratio_pixmap_label.h"
#include "gcreceiver.h"

class FieldSpaceManager
{
public:
	FieldSpaceManager(const int, const int);
	~FieldSpaceManager();
	void setObjectPos(const int, const int, const int, const int);
	void clear(void);
private:
	static constexpr int EXIST = 1;
	static constexpr int EMPTY = 0;
	const int grid_num_x;
	const int grid_num_y;
	std::vector<std::vector<int>> grid_map;
	const int field_width;
	const int field_height;
};

/*
 * Field parameters.
 * See Law 1 of rule book(2018) at http://www.robocuphumanoid.org/wp-content/uploads/RCHL-2018-Rules-Proposal_changesMarked_final.pdf
 */
class FieldParameter
{
	// unit: meter
public:
	FieldParameter() :
		field_length(9.0),
		field_width(6.0),
		goal_depth(0.6),
		goal_width(2.6),
		goal_height(1.8),
		goal_area_length(1.0),
		goal_area_width(5.0),
		penalty_mark_distance(2.1),
		center_circle_diameter(1.5),
		border_strip_width(0.7)
	{
	}
	~FieldParameter() {};
	const double field_length;
	const double field_width;
	const double goal_depth;
	const double goal_width;
	const double goal_height;
	const double goal_area_length;
	const double goal_area_width;
	const double penalty_mark_distance;
	const double center_circle_diameter;
	const double border_strip_width;
};

class FieldParameterInt
{
	// unit: centimeter
public:
	FieldParameterInt(FieldParameter param) :
		field_length(static_cast<int>(param.field_length * 100)),
		field_width(static_cast<int>(param.field_width * 100)),
		goal_depth(static_cast<int>(param.goal_depth * 100)),
		goal_width(static_cast<int>(param.goal_width * 100)),
		goal_height(static_cast<int>(param.goal_height * 100)),
		goal_area_length(static_cast<int>(param.goal_area_length * 100)),
		goal_area_width(static_cast<int>(param.goal_area_width * 100)),
		penalty_mark_distance(static_cast<int>(param.penalty_mark_distance * 100)),
		center_circle_diameter(static_cast<int>(param.center_circle_diameter * 100)),
		border_strip_width(static_cast<int>(param.border_strip_width * 100))
	{
	}
	~FieldParameterInt() {};
	const int field_length;
	const int field_width;
	const int goal_depth;
	const int goal_width;
	const int goal_height;
	const int goal_area_length;
	const int goal_area_width;
	const int penalty_mark_distance;
	const int center_circle_diameter;
	const int border_strip_width;
};

class PositionMarker {
public:
	PositionMarker() : self_conf(0.0), ball_conf(0.0), colornum(0), enable_pos(false), enable_ball(false), enable_goal_pole{false, false} { color[0] = '\0'; }
	double self_conf;
	double ball_conf;
	int colornum;
	bool enable_pos;
	bool enable_ball;
	bool enable_goal_pole[2];
	struct tm lastReceiveTime;
	char color[20];
	Pos pos; /* self position */
	Pos ball; /* ball position */
	Pos goal_pole[2]; /* goal pole position */
	std::string message;
};

class Robot {
public:
	QLabel *name;
	QLabel *string;
	QLabel *cf_own;
	QLabel *cf_ball;
	QProgressBar *cf_own_bar;
	QProgressBar *cf_ball_bar;
	QProgressBar *time_bar;
};

class LogDataRobotComm {
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

class LogData
{
public:
	LogData() : type(0), score1(0), score2(0), remaining_time(0) { }
	~LogData() { }
	int type;
	LogDataRobotComm robot_comm;
	int score1;
	int score2;
	int remaining_time;
	char time_str[100];
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
	Capture *capture;
	LogWriter log_writer;
	std::vector<UdpServer *> th;
	GCReceiver *gc_thread;
	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *videoMenu;
	QAction *loadLogFileAction;
	QAction *viewGoalPostAction;
	QStatusBar *statusBar;
	QCheckBox *reverse;
	QCheckBox *viewGoalpostCheckBox;
	QCheckBox *viewSelfPosConfCheckBox;
	QPushButton *log1Button, *log2Button, *log5Button;
	QPushButton *recordButton;
	QSettings *settings;
	QString filenameDrag;
	QWidget *window;
	AspectRatioPixmapLabel *image;
	QLabel *log_step;
	QLCDNumber *time_display;
	QLCDNumber *score_display;
	QPixmap map;
	QPixmap origin_map;
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
	std::vector<Robot> robot;
	std::vector<QLabel *> idLabel;
	std::vector<ClickWidget *>robotState;
	std::vector<QGridLayout *>idLayout;
	std::vector<LogData> log_data;
	bool fLogging;
	bool fReverse;
	bool fViewGoalpost;
	bool fPauseLog;
	bool fRecording;
	bool fViewSelfPosConf;
	int updateMapTimerId;
	int score_team1;
	int score_team2;
	unsigned int log_count;
	const int max_robot_num;
	int logo_pos_x, logo_pos_y;
	int log_speed;
	int select_robot_num;
	struct tm last_select_time;
	FieldParameterInt field_param;
	FieldSpaceManager field_space;
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
	void createMenus(void);
	void drawTeamMarker(QPainter &, const int, const int);
	void drawRobotMarker(QPainter &, const int, const int, const double, const int, const QColor, const double);
	void drawRobotInformation(QPainter &, const int, const int, const double, const int, const QColor, const double, const double, const std::string);
	void drawBallMarker(QPainter &, const int, const int, const int, const int, const int, const int);
	void drawGoalPostMarker(QPainter &, const int, const int, const int, const int);
	void drawHighlightCircle(QPainter &, const int, const int);

public:
	Interface();
	~Interface();
	void drawField(void);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
	void decodeUdp(struct comm_info_T, Robot *, int num);
	void updateMap(void);

private slots:
	void decodeData1(struct comm_info_T);
	void decodeData2(struct comm_info_T);
	void decodeData3(struct comm_info_T);
	void decodeData4(struct comm_info_T);
	void decodeData5(struct comm_info_T);
	void decodeData6(struct comm_info_T);
	void setRemainingTime(int);
	void setScore1(int);
	void setScore2(int);
	void selectRobot1(void);
	void selectRobot2(void);
	void selectRobot3(void);
	void selectRobot4(void);
	void selectRobot5(void);
	void selectRobot6(void);
	void reverseField(int);
	void viewGoalpost(int);
	void viewGoalpost(bool);
	void viewSelfPosConf(int);
	void loadLogFile(void);
	void updateLog(void);
	void logSpeed1(void);
	void logSpeed2(void);
	void logSpeed5(void);
	void pausePlayingLog(void);
	void changeLogPosition(void);
	void captureButtonSlot(void);
	void updateCameraDevice(QAction *);
	void showRecordTime(QString);
	void setRecordButtonText(QString);
};

#endif // INTERFACE_H

