
#include <QtGui>
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>

#include "pos_types.h"
#include "interface.h"

Interface::Interface(): fLogging(true), fReverse(false), max_robot_num(6)
{
	qRegisterMetaType<comm_info_T>("comm_info_T");
	setAcceptDrops(true);
	log.setEnable();
	positions = std::vector<PositionMarker>(max_robot_num);

	settings = new QSettings("./config.ini", QSettings::IniFormat);
	initializeConfig();
	const int fw = settings->value("field_image/width").toInt();
	const int lw = settings->value("team_logo/width").toInt();
	const int fh = settings->value("field_image/height").toInt();
	const int lh = settings->value("team_logo/height").toInt();
	logo_pos_x = fw / 4 - lw / 2;
	logo_pos_y = fh / 2 - lh / 2;

	/* Run receive thread */
	const int base_udp_port = settings->value("network/port").toInt();
	for(int i = 0; i < max_robot_num; i++)
		th.push_back(new UdpServer(base_udp_port + i));

	createWindow();
	connection();

	updateMapTimerId = startTimer(1000); /* timer by 1000msec */
	const QString field_image_name = settings->value("field_image/name").toString();
	const QString team_logo_image_name = settings->value("team_logo/name").toString();
	loadImage(field_image_name, team_logo_image_name);

	this->setWindowTitle("Humanoid League Game Monitor");
}

Interface::~Interface()
{
}

void Interface::initializeConfig(void)
{
	/* Field image file */
	settings->setValue("field_image/name", settings->value("field_image/name", "hlfield.png"));
	/* 740x540 pixel: field image size */
	settings->setValue("field_image/width" , settings->value("field_image/width", 740));
	settings->setValue("field_image/height", settings->value("field_image/height", 540));
	/* Team logo image file */
	settings->setValue("team_logo/name", settings->value("team_logo/name", "citbrains_logo.png"));
	/* Team logo image size */
	settings->setValue("team_logo/width", settings->value("team_logo/width", "200"));
	settings->setValue("team_logo/height", settings->value("team_logo/height", "200"));
	/* field size 10000x7000 milli meter? (map size in robot used) */
	settings->setValue("field_size/x", settings->value("field_size/x", 10000));
	settings->setValue("field_size/y", settings->value("field_size/y", 7000));
	/* marker */
	settings->setValue("marker/robot_size", settings->value("marker/robot_size", 3));
	settings->setValue("marker/ball_size", settings->value("marker/ball_size", 6));
	settings->setValue("marker/length", settings->value("marker/length", 24));
	settings->setValue("marker/rear_length", settings->value("marker/rear_length", 10));
	settings->setValue("marker/font_offset", settings->value("marker/font_offset", 10));
	settings->setValue("marker/time_up_limit", settings->value("marker/time_up_limit", 30));
	/* using UDP communication port offset */
	settings->setValue("network/port", settings->value("network/port", 7110));
}

void Interface::createWindow(void)
{
	window     = new QWidget;
	reverse    = new QCheckBox("Reverse field");
	image      = new QLabel;
	id         = new QLabel("ID");
	name       = new QLabel("Name");
	voltage    = new QLabel("Voltage");
	fps        = new QLabel("FPS");
	string     = new QLabel("Common String");
	cf_own     = new QLabel("Reliability of self pos");
	cf_ball    = new QLabel("Reliability of ball pos");
	loadLogButton = new QPushButton("Load log file");
	mainLayout = new QGridLayout;
	checkLayout = new QHBoxLayout;
	labelLayout = new QGridLayout;
	for(int i = 0; i < max_robot_num; i++)
		idLayout.push_back(new QGridLayout);

	checkLayout->addWidget(reverse);
	checkLayout->addWidget(loadLogButton);

	pal_state_bgcolor.setColor(QPalette::Window, QColor("#D0D0D0"));
	pal_red.   setColor(QPalette::Window, QColor("#FF8E8E"));
	pal_green. setColor(QPalette::Window, QColor("#8EFF8E"));
	pal_blue.  setColor(QPalette::Window, QColor("#8E8EFF"));
	pal_black. setColor(QPalette::Window, QColor("#000000"));
	pal_orange.setColor(QPalette::Window, QColor("#FFA540"));

	const int time_limit = settings->value("marker/time_up_limit").toInt();
	for(int i = 0; i < max_robot_num; i++) {
		robotState.push_back(new QWidget());
		robotState[i]->setAutoFillBackground(true);
		robotState[i]->setPalette(pal_state_bgcolor);
		robotState[i]->setFixedWidth(200);
		idLabel.push_back(new QLabel());
		idLabel[i]->setNum(i + 1);
		struct robot robo;
		robo.name = new QLabel();
		robo.voltage = new QLabel();
		robo.fps = new QLabel();
		robo.string = new QLabel();
		robo.cf_own = new QLabel();
		robo.cf_ball = new QLabel();
		robo.cf_own_bar = new QProgressBar();
		robo.cf_ball_bar = new QProgressBar();
		robo.time_bar = new QProgressBar();
		robo.cf_own_bar->setRange(0, 100);
		robo.cf_ball_bar->setRange(0, 100);
		robo.time_bar->setRange(0, time_limit);
		robot.push_back(robo);
	}

	for(int i = 0; i < max_robot_num; i++) {
		idLayout[i]->addWidget(idLabel[i], 1, 1);
		idLayout[i]->addWidget(robot[i].name, 2, 1);
		/*
		idLayout[i]->addWidget(robot[i].voltage, 3, 1);
		idLayout[i]->addWidget(robot[i].fps, 4, 1);
		idLayout[i]->addWidget(robot[i].string, 5, 1);
		idLayout[i]->addWidget(robot[i].cf_own_bar, 6, 1);
		idLayout[i]->addWidget(robot[i].cf_ball_bar, 7, 1);
		idLayout[i]->addWidget(robot[i].cf_own, 6, 2);
		idLayout[i]->addWidget(robot[i].cf_ball, 7, 2);
		*/
		idLayout[i]->addWidget(robot[i].string, 3, 1);
		idLayout[i]->addWidget(robot[i].cf_own_bar, 4, 1);
		idLayout[i]->addWidget(robot[i].cf_ball_bar, 5, 1);
		idLayout[i]->addWidget(robot[i].time_bar, 6, 1);
		idLayout[i]->addWidget(robot[i].cf_own, 4, 2);
		idLayout[i]->addWidget(robot[i].cf_ball, 5, 2);
		robotState[i]->setLayout(idLayout[i]);
	}

	for(int i = 0, j = 0, k = 0; i < max_robot_num; i++) {
		labelLayout->addWidget(robotState[i], k + 1, j + 1);
		if(++j == 2) { k++; j = 0; }
	}

	mainLayout->addLayout(checkLayout, 1, 1, 1, 2);
	mainLayout->addWidget(image, 2, 1);
	mainLayout->addLayout(labelLayout, 2, 2);

	window->setLayout(mainLayout);
	setCentralWidget(window);
}

void Interface::loadImage(QString field_image_name, QString team_logo_image_name)
{
	QImage field_image_buf(field_image_name);
	if(field_image_buf.isNull()) {
		std::cerr << "Error: couldn\'t open image file" << std::endl;
		return;
	}
	origin_map = QPixmap::fromImage(field_image_buf);
	map = origin_map;
	image->setPixmap(map);

	QImage logo_image_buf(team_logo_image_name);
	if(logo_image_buf.isNull()) {
		std::cerr << "Error: couldn\'t open image file" << std::endl;
		return;
	}
	team_logo_map = QPixmap::fromImage(logo_image_buf);
}

void Interface::loadImage(const char *image_filename)
{
	QImage image_buf(image_filename);
	if(image_buf.isNull()) {
		std::cerr << "Error: couldn\'t open image file" << std::endl;
		return;
	}
	origin_map = QPixmap::fromImage(image_buf);
	map = origin_map;
	image->setPixmap(QPixmap::fromImage(image_buf));
}

void Interface::dragEnterEvent(QDragEnterEvent *e)
{
	if(e->mimeData()->hasFormat("text/uri-list")) {
		e->acceptProposedAction();
	}
}

void Interface::dropEvent(QDropEvent *e)
{
	filenameDrag = e->mimeData()->urls().first().toLocalFile();
}

void Interface::connection(void)
{
	connect(th[0], SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData1(struct comm_info_T)));
	connect(th[1], SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData2(struct comm_info_T)));
	connect(th[2], SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData3(struct comm_info_T)));
	connect(th[3], SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData4(struct comm_info_T)));
	connect(th[4], SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData5(struct comm_info_T)));
	connect(th[5], SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData6(struct comm_info_T)));
	connect(reverse, SIGNAL(stateChanged(int)), this, SLOT(reverseField(int)));
	connect(loadLogButton, SIGNAL(clicked()), this, SLOT(loadLogFile()));
}

void Interface::decodeData1(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot[0], 0);
}

void Interface::decodeData2(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot[1], 1);
}

void Interface::decodeData3(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot[2], 2);
}

void Interface::decodeData4(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot[3], 3);
}

void Interface::decodeData5(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot[4], 4);
}

void Interface::decodeData6(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot[5], 5);
}

void Interface::decodeUdp(struct comm_info_T comm_info, struct robot *robot_data, int num)
{
	char buf[2048];
	char color_str[100];
	int color, id;
	double voltage;

	/* MAGENTA, CYAN */
	color = (int)(comm_info.id & 0x80) >> 7;
	id    = (int)(comm_info.id & 0x7F);

	/* record time of receive data */
	time_t timer;
	struct tm *local_time;
	timer = time(NULL);
	local_time = localtime(&timer);
	positions[num].lastReceiveTime = *local_time;

	/* ID and Color */
	sprintf(color_str, "%s %d", ((color == MAGENTA) ? "MAGENTA" : "CYAN"), id);
	robot_data->name->setText(color_str);
	/* Voltage */
	voltage = (comm_info.voltage << 3) / 100.0;
	//sprintf(buf, "%.2lf", voltage);
	//robot_data->voltage->setText(buf);
	/* FPS */
	//robot_data->fps->setNum(comm_info.fps);
	/* Self-position confidence */
	robot_data->cf_own->setNum(comm_info.cf_own);
	robot_data->cf_own_bar->setValue(comm_info.cf_own);
	/* Ball position confidence */
	robot_data->cf_ball->setNum(comm_info.cf_ball);
	robot_data->cf_ball_bar->setValue(comm_info.cf_ball);
	const int time_limit = settings->value("marker/time_up_limit").toInt();
	robot_data->time_bar->setValue(time_limit);
	/* Role and message */
	if(strstr((const char *)comm_info.command, "Attacker")) {
		/* Red */
		robotState[num]->setPalette(pal_red);
		strcpy(positions[num].color, "red");
	} else if(strstr((const char *)comm_info.command, "Neutral")) {
		/* Green */
		robotState[num]->setPalette(pal_green);
		strcpy(positions[num].color, "green");
	} else if(strstr((const char *)comm_info.command, "Defender")) {
		/* Blue */
		robotState[num]->setPalette(pal_blue);
		strcpy(positions[num].color, "blue");
	} else if(strstr((const char *)comm_info.command, "Keeper")) {
		/* Orange */
		robotState[num]->setPalette(pal_orange);
		strcpy(positions[num].color, "orange");
	} else {
		/* Black */
		robotState[num]->setPalette(pal_state_bgcolor);
		strcpy(positions[num].color, "black");
	}
	robot_data->string->setText((char *)comm_info.command);

	/* Decode robot position */
	if(getCommInfoObject(comm_info.object[1], &(positions[num].pos))) {
		positions[num].pos = globalPosToImagePos(positions[num].pos);
		positions[num].enable_pos  = true;
	} else {
		positions[num].enable_pos = false;
	}
	/* Decode ball position */
	if(getCommInfoObject(comm_info.object[0], &(positions[num].ball))) {
		positions[num].ball = globalPosToImagePos(positions[num].ball);
		positions[num].enable_ball = true;
	} else {
		positions[num].enable_ball = false;
	}
	updateMap();
	log.write(num + 1, color_str, (int)comm_info.fps, (double)voltage,
		(int)positions[num].pos.x, (int)positions[num].pos.y, (float)positions[num].pos.th,
		(int)positions[num].ball.x, (int)positions[num].ball.y, (char *)comm_info.command);
}

Pos Interface::globalPosToImagePos(Pos gpos)
{
	Pos ret_pos;
	const int field_image_width = settings->value("field_image/width").toInt();
	const int field_image_height = settings->value("field_image/height").toInt();
	const int field_size_x = settings->value("field_size/x").toInt();
	const int field_size_y = settings->value("field_size/y").toInt();
	ret_pos.x =
		field_image_width - (int)((double)gpos.x * ((double)field_image_width / (double)field_size_x) + ((double)field_image_width / 2));
	ret_pos.y =
		                    (int)((double)gpos.y * ((double)field_image_height / (double)field_size_y) + ((double)field_image_height / 2));
	ret_pos.th = -gpos.th + M_PI;
	return ret_pos;
}

void Interface::setParamFromFile(std::vector<std::string> lines)
{
	for(auto line : lines) {
		struct log_data_t buf;
		QString qstr = QString(line.c_str());
		QStringList list = qstr.split(QChar(','));

		int size = list.size();
		if(size < 1) continue;
		strcpy(buf.time_str, list.at(0).toStdString().c_str());
		if(size < 2) continue;
		buf.id = list.at(1).toInt();
		if(size < 3) continue;
		strcpy(buf.color_str, list.at(2).toStdString().c_str());
		if(size < 4) continue;
		buf.fps = list.at(3).toInt();
		if(size < 5) continue;
		buf.voltage = list.at(4).toDouble();
		if(size < 6) continue;
		buf.x = list.at(5).toInt();
		if(size < 7) continue;
		buf.y = list.at(6).toInt();
		if(size < 8) continue;
		buf.theta = list.at(7).toDouble();
		if(size < 9) continue;
		buf.ball_x = list.at(8).toInt();
		if(size < 10) continue;
		buf.ball_y = list.at(9).toInt();
		if(size < 11) continue;
		strcpy(buf.msg, list.at(10).toStdString().c_str());
		log_data.push_back(buf);
	}

	log_count = 0;
	setData(log_data[log_count]);
	QString before = QString(log_data[log_count++].time_str);
	QString after = QString(log_data[log_count].time_str);
	int interval = getInterval(before, after);
	QTimer::singleShot(interval, this, SLOT(updateLog()));
}

int Interface::getInterval(QString before, QString after)
{
	QStringList list_before = before.split(QChar(':'));
	QStringList list_after = after.split(QChar(':'));
	int h = list_after.at(0).toInt() - list_before.at(0).toInt();
	int m = list_after.at(1).toInt() - list_before.at(1).toInt();
	int s = list_after.at(2).toInt() - list_before.at(2).toInt();
	return ((h * 60 + m) * 60 + s) * 1000;
}

void Interface::updateLog(void)
{
	setData(log_data[log_count]);
	if(log_count + 1 >= log_data.size()) return;
	QString before = QString(log_data[log_count++].time_str);
	QString after = QString(log_data[log_count].time_str);
	int interval = getInterval(before, after);
	QTimer::singleShot(interval, this, SLOT(updateLog()));
}

void Interface::setData(struct log_data_t data)
{
	int num = data.id - 1;
	struct robot *robot_data = &robot[num];

	/* ID and Color */
	robot_data->name->setText(data.color_str);
	/* Voltage */
	//robot_data->voltage->setNum(data.voltage);
	/* FPS */
	//robot_data->fps->setNum(data.fps);
	/* Self-position confidence */
	robot_data->cf_own->setNum(0);
	robot_data->cf_own_bar->setValue(0);
	/* Ball position confidence */
	robot_data->cf_ball->setNum(0);
	robot_data->cf_ball_bar->setValue(0);
	robot_data->time_bar->setValue(0);
	/* Role and message */
	char *msg = data.msg;
	if(strstr((const char *)msg, "Attacker")) {
		/* Red */
		robotState[num]->setPalette(pal_red);
		strcpy(positions[num].color, "red");
	} else if(strstr((const char *)msg, "Neutral")) {
		/* Green */
		robotState[num]->setPalette(pal_green);
		strcpy(positions[num].color, "green");
	} else if(strstr((const char *)msg, "Defender")) {
		/* Blue */
		robotState[num]->setPalette(pal_blue);
		strcpy(positions[num].color, "blue");
	} else if(strstr((const char *)msg, "Keeper")) {
		/* Orange */
		robotState[num]->setPalette(pal_orange);
		strcpy(positions[num].color, "orange");
	} else {
		/* Black */
		robotState[num]->setPalette(pal_state_bgcolor);
		strcpy(positions[num].color, "black");
	}
	robot_data->string->setText((char *)msg);

	positions[num].enable_pos  = true;
	positions[num].enable_ball = true;

	positions[num].pos.x = data.x;
	positions[num].pos.y = data.y;
	positions[num].pos.th = data.theta;
	positions[num].ball.x = data.ball_x;
	positions[num].ball.y = data.ball_y;

	updateMap();
}

void Interface::updateMap(void)
{
	char buf[2048];
	time_t timer;
	struct tm *local_time;
	timer = time(NULL);
	local_time = localtime(&timer);
	const int time_limit = settings->value("marker/time_up_limit").toInt();

	/* Create new image for erase previous position marker */
	map = origin_map;
	QPainter paint(&map);
	paint.drawPixmap(logo_pos_x, logo_pos_y, team_logo_map);

	/* draw position marker on image */
	for(int i = 0; i < max_robot_num; i++) {
		int self_x = positions[i].pos.x;
		int self_y = positions[i].pos.y;
		double theta = positions[i].pos.th;
		if(fReverse) {
			self_x = settings->value("field_image/width").toInt()  - self_x;
			self_y = settings->value("field_image/height").toInt() - self_y;
			theta = theta + M_PI;
		}
		int ball_x = positions[i].ball.x;
		int ball_y = positions[i].ball.y;
		if(fReverse) {
			ball_x = settings->value("field_image/width").toInt()  - ball_x;
			ball_y = settings->value("field_image/height").toInt() - ball_y;
		}
		if(positions[i].enable_pos == true) {
			const int elapsed = (local_time->tm_min - positions[i].lastReceiveTime.tm_min) * 60 + (local_time->tm_sec - positions[i].lastReceiveTime.tm_sec);
			if(elapsed > time_limit) {
				positions[i].enable_pos = false;
				positions[i].enable_ball = false;
				robotState[i]->setPalette(pal_state_bgcolor);
				robot[i].name->clear();
				robot[i].voltage->clear();
				robot[i].fps->clear();
				robot[i].string->clear();
				robot[i].cf_own->clear();
				robot[i].cf_ball->clear();
				robot[i].cf_own_bar->setValue(0);
				robot[i].cf_ball_bar->setValue(0);
				robot[i].time_bar->setValue(0);
				continue;
			}
			paint.setBrush(Qt::red);
			/*
			 * self-position maker color:
			 *  Attacker: Red
			 *  Other   : Black
			 */
			const int robot_marker_size = settings->value("marker/robot_size").toInt();
			if(!strcmp(positions[i].color, "red")) {
				paint.setPen(QPen(QColor(0xFF, 0x00, 0x00), robot_marker_size));
			} else {
				paint.setPen(QPen(QColor(0x00, 0x00, 0x00), robot_marker_size));
			}
			/* draw robot position */
			paint.drawPoint(self_x, self_y);
			/* calclate robot theta */
			const int half_length = settings->value("marker/length").toInt() / 2;
			const int half_rear_length = settings->value("marker/rear_length").toInt() / 2;
			const int front_x = (int)(self_x + half_length * cos(theta));
			const int front_y = (int)(self_y + half_length * sin(theta));
			const int rear_x = (int)(self_x + half_length * cos(theta + M_PI));
			const int rear_y = (int)(self_y + half_length * sin(theta + M_PI));
			const int rear_left_x = (int)(rear_x + half_rear_length * cos(theta + M_PI / 2));
			const int rear_left_y = (int)(rear_y + half_rear_length * sin(theta + M_PI / 2));
			const int rear_right_x = (int)(rear_x + half_rear_length * cos(theta - M_PI / 2));
			const int rear_right_y = (int)(rear_y + half_rear_length * sin(theta - M_PI / 2));
			paint.drawLine(front_x, front_y, rear_left_x, rear_left_y);
			paint.drawLine(front_x, front_y, rear_right_x, rear_right_y);
			paint.drawLine(rear_left_x, rear_left_y, rear_right_x, rear_right_y);
			sprintf(buf, "%d", i + 1);
			const int font_offset = settings->value("marker/font_offset").toInt();
			paint.drawText(QPoint(self_x - font_offset, self_y - font_offset), buf);
			if(positions[i].enable_ball == true) {
				/* draw ball position as orange */
				const int ball_marker_size = settings->value("marker/ball_size").toInt();
				paint.setPen(QPen(QColor(0xFF, 0xA5, 0x00), ball_marker_size));
				paint.drawPoint(ball_x, ball_y);
				sprintf(buf, "%d", i + 1);
				paint.drawText(QPoint(ball_x - font_offset, ball_y - font_offset), buf);
				paint.setPen(QPen(QColor(0xFF, 0xA5, 0x00), 1));
				paint.drawLine(front_x, front_y, ball_x, ball_y);
			}
			robot[i].time_bar->setValue(elapsed);
		}
	}
	image->setPixmap(map);
}

void Interface::timerEvent(QTimerEvent *e)
{
	if(e->timerId() == updateMapTimerId) {
		updateMap();
	}
}

void Interface::reverseField(int state)
{
	const int fw = settings->value("field_image/width").toInt();
	const int lw = settings->value("team_logo/width").toInt();
	if(state == Qt::Checked) {
		fReverse = true;
		logo_pos_x = fw / 2 + fw / 4 - lw / 2;
	} else {
		fReverse = false;
		logo_pos_x = fw / 4 - lw / 2;
	}
	updateMap();
}

void Interface::loadLogFile(void)
{
	QString fileName = QFileDialog::getOpenFileName(this, "log file", "./log", "*.log");
	std::ifstream ifs(fileName.toStdString());
	if(ifs.fail()) {
		std::cerr << "file open error" << std::endl;
		return;
	}
	std::string line;
	std::vector<std::string> lines;
	while(getline(ifs, line)) {
		lines.push_back(line);
	}
	setParamFromFile(lines);
}

