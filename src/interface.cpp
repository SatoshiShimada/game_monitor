
#include <QtGui>
#include <iostream>
#include <cstring>
#include <cmath>

#include "pos_types.h"
#include "interface.h"

Interface::Interface(): fLogging(true), fReceive(true), fReverse(false), robot_num(6)
{
	qRegisterMetaType<comm_info_T>("comm_info_T");
	setAcceptDrops(true);
	log.setEnable();
	positions = std::vector<PositionMarker>(robot_num);

	settings = new QSettings("./config.ini", QSettings::IniFormat);
	initializeConfig();

	createWindow();
	connection();

	loadImage("hlfield.png");

	/* Run receive thread */
	for(int i = 0; i < robot_num; i++)
		th[i]->start();

	this->setWindowTitle("Humanoid League Game Monitor");
}

Interface::~Interface()
{
}

void Interface::initializeConfig(void)
{
	/* 740x540 pixel: field image size */
	settings->setValue("field_image_width" , settings->value("field_image_width", 740));
	settings->setValue("field_image_height", settings->value("field_image_height", 540));
	/* field size 10000x7000 milli meter? (map size in robot used) */
	settings->setValue("field_size_x", settings->value("field_size_x", 10000));
	settings->setValue("field_size_y", settings->value("field_size_y", 7000));
	/* marker */
	settings->setValue("marker/robot_size", settings->value("marker/robot_size", 5 * 2));
	settings->setValue("marker/ball_size", settings->value("marker/ball_size", 3 * 2));
	settings->setValue("marker/length", settings->value("marker/length", 10 * 2));
	settings->setValue("marker/rear_length", settings->value("marker/rear_length", 4 * 2));
	settings->setValue("marker/font_offset", settings->value("marker/font_offset", 10));
	/* using UDP communication port offset */
	settings->setValue("network/port", settings->value("network/port", 7110));
}

void Interface::createWindow(void)
{
	window     = new QWidget;
	for(int i = 0; i < robot_num; i++)
		th.push_back(new UdpThread(settings->value("network/port").toInt() + i));
	receive    = new QCheckBox("Receive data");
	reverse    = new QCheckBox("Reverse field");
	image      = new QLabel;
	id         = new QLabel("ID");
	name       = new QLabel("Name");
	voltage    = new QLabel("Voltage");
	fps        = new QLabel("FPS");
	string     = new QLabel("Common String");
	cf_own     = new QLabel("Reliability of self pos");
	cf_ball    = new QLabel("Reliability of ball pos");
	mainLayout = new QGridLayout;
	checkLayout = new QHBoxLayout;
	labelLayout = new QGridLayout;
	for(int i = 0; i < robot_num; i++)
		idLayout.push_back(new QGridLayout);

	receive->setCheckState(Qt::Checked);
	checkLayout->addWidget(receive);
	checkLayout->addWidget(reverse);

	pal_state_bgcolor.setColor(QPalette::Window, QColor("#D0D0D0"));
	pal_red.   setColor(QPalette::Window, QColor("#FF8E8E"));
	pal_green. setColor(QPalette::Window, QColor("#8EFF8E"));
	pal_blue.  setColor(QPalette::Window, QColor("#8E8EFF"));
	pal_black. setColor(QPalette::Window, QColor("#000000"));
	pal_orange.setColor(QPalette::Window, QColor("#FFA540"));

	for(int i = 0; i < robot_num; i++) {
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
		robo.cf_own_bar->setRange(0, 100);
		robo.cf_ball_bar->setRange(0, 100);
		robot.push_back(robo);
	}

	for(int i = 0; i < robot_num; i++) {
		idLayout[i]->addWidget(idLabel[i], 1, 1);
		idLayout[i]->addWidget(robot[i].name, 2, 1);
		idLayout[i]->addWidget(robot[i].voltage, 3, 1);
		idLayout[i]->addWidget(robot[i].fps, 4, 1);
		idLayout[i]->addWidget(robot[i].string, 5, 1);
		idLayout[i]->addWidget(robot[i].cf_own_bar, 6, 1);
		idLayout[i]->addWidget(robot[i].cf_ball_bar, 7, 1);
		idLayout[i]->addWidget(robot[i].cf_own, 6, 2);
		idLayout[i]->addWidget(robot[i].cf_ball, 7, 2);
		robotState[i]->setLayout(idLayout[i]);
	}

	for(int i = 0, j = 0, k = 0; i < robot_num; i++) {
		labelLayout->addWidget(robotState[i], k + 1, j + 1);
		if(++j == 2) { k++; j = 0; }
	}

	mainLayout->addLayout(checkLayout, 1, 1, 1, 2);
	mainLayout->addWidget(image, 2, 1);
	mainLayout->addLayout(labelLayout, 2, 2);

	window->setLayout(mainLayout);
	setCentralWidget(window);
}

void Interface::loadImage(QString image_filename)
{
	QImage image_buf(image_filename);
	if(image_buf.isNull()) {
		std::cerr << "Error: couldn\'t open image file" << std::endl;
		return;
	}
	origin_map = QPixmap::fromImage(image_buf);
	map = origin_map;
	image->setPixmap(map);
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
	connect(receive, SIGNAL(stateChanged(int)), this, SLOT(receiveStateChange(int)));
	connect(reverse, SIGNAL(stateChanged(int)), this, SLOT(reverseField(int)));
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

	/* ID and Color */
	sprintf(color_str, "%s %d", ((color == MAGENTA) ? "MAGENTA" : "CYAN"), id);
	robot_data->name->setText(color_str);
	/* Voltage */
	voltage = (comm_info.voltage << 3) / 100.0;
	sprintf(buf, "%.2lf", voltage);
	robot_data->voltage->setText(buf);
	/* FPS */
	robot_data->fps->setNum(comm_info.fps);
	/* Self-position confidence */
	robot_data->cf_own->setNum(comm_info.cf_own);
	robot_data->cf_own_bar->setValue(comm_info.cf_own);
	/* Ball position confidence */
	robot_data->cf_ball->setNum(comm_info.cf_ball);
	robot_data->cf_ball_bar->setValue(comm_info.cf_ball);
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
	/* Create new image for erase previous position marker */
	map = origin_map;
	QPainter paint(&map);

	positions[num].enable_pos  = true;
	positions[num].enable_ball = true;
	positions[num].lastReceive = 0;

	/* Decode robot position */
	if(getCommInfoObject(comm_info.object[1], &(positions[num].pos)) == false) {
		positions[num].enable_pos = false;
	} else {
		positions[num].pos.x =
			settings->value("field_image_width").toInt() - (int)((double)positions[num].pos.x * ((double)settings->value("field_image_width").toInt() / (double)settings->value("field_size_x").toInt()) + ((double)settings->value("field_image_width").toInt() / 2));
		positions[num].pos.y =
			(int)((double)positions[num].pos.y * ((double)settings->value("field_image_height").toInt() / (double)settings->value("field_size_y").toInt()) + ((double)settings->value("field_image_height").toInt() / 2));
		positions[num].pos.th = positions[num].pos.th * -1 + M_PI;
		if(fReverse) {
			positions[num].pos.x = settings->value("field_image_width").toInt()  - positions[num].pos.x;
			positions[num].pos.y = settings->value("field_image_height").toInt() - positions[num].pos.y;
			positions[num].pos.th = positions[num].pos.th + M_PI;
		}
	}
	/* Decode ball position */
	if(getCommInfoObject(comm_info.object[0], &(positions[num].ball)) == false) {
		positions[num].enable_ball = false;
	} else {
		positions[num].ball.x =
			settings->value("field_image_width").toInt() - (int)((double)positions[num].ball.x * ((double)settings->value("field_image_width").toInt() / (double)settings->value("field_size_x").toInt()) + ((double)settings->value("field_image_width").toInt() / 2));
		positions[num].ball.y =
			(int)((double)positions[num].ball.y * ((double)settings->value("field_image_height").toInt() / (double)settings->value("field_size_y").toInt()) + ((double)settings->value("field_image_height").toInt() / 2));
		if(fReverse) {
			positions[num].ball.x = settings->value("field_image_width").toInt()  - positions[num].ball.x;
			positions[num].ball.y = settings->value("field_image_height").toInt() - positions[num].ball.y;
		}
	}

	/* draw position marker on image */
	for(int i = 0; i < 6; i++) {
		int self_x = positions[i].pos.x;
		int self_y = positions[i].pos.y;
		int ball_x = positions[i].ball.x;
		int ball_y = positions[i].ball.y;
		if(positions[i].lastReceive >= 10) {
			positions[i].enable_pos  = false;
			positions[i].enable_ball = false;
		}
		if(positions[i].enable_pos == true) {
			paint.setBrush(Qt::red);
			/*
			 * self-position maker color:
			 *  Attacker: Red
			 *  Other   : Black
			 */
			if(!strcmp(positions[i].color, "red")) {
				paint.setPen(QPen(QColor(0xFF, 0x00, 0x00), settings->value("marker/robot_size").toInt()));
			} else {
				paint.setPen(QPen(QColor(0x00, 0x00, 0x00), settings->value("marker/robot_size").toInt()));
			}
			/* draw robot position */
			paint.drawPoint(self_x, self_y);
			/* calclate robot theta */
			int half_length = settings->value("marker/length").toInt() / 2;
			int half_rear_length = settings->value("marker/rear_length").toInt() / 2;
			double theta = positions[i].pos.th;
			int front_x = (int)(self_x + half_length * cos(theta));
			int front_y = (int)(self_y + half_length * sin(theta));
			int rear_x = (int)(self_x + half_length * cos(theta + M_PI));
			int rear_y = (int)(self_y + half_length * sin(theta + M_PI));
			int rear_left_x = (int)(rear_x + half_rear_length * cos(theta + M_PI / 2));
			int rear_left_y = (int)(rear_y + half_rear_length * sin(theta + M_PI / 2));
			int rear_right_x = (int)(rear_x + half_rear_length * cos(theta - M_PI / 2));
			int rear_right_y = (int)(rear_y + half_rear_length * sin(theta - M_PI / 2));
			paint.drawLine(front_x, front_y, rear_left_x, rear_left_y);
			paint.drawLine(front_x, front_y, rear_right_x, rear_right_y);
			paint.drawLine(rear_left_x, rear_left_y, rear_right_x, rear_right_y);
			sprintf(buf, "%d", i);
			int font_offset = settings->value("marker/font_offset").toInt();
			paint.drawText(QPoint(self_x - font_offset, self_y - font_offset), buf);
			if(positions[i].enable_ball == true) {
				/* draw ball position as orange */
				paint.setPen(QPen(QColor(0xFF, 0xA5, 0x00), settings->value("marker/ball_size").toInt()));
				paint.drawPoint(ball_x, ball_y);
				sprintf(buf, "%d", i);
				paint.drawText(QPoint(ball_x - font_offset, ball_y - font_offset), buf);
			}
		}
		positions[i].lastReceive++;
	}
	image->setPixmap(map);
	log.write(num + 1, color_str, (int)comm_info.fps, (double)voltage,
		(int)positions[num].pos.x, (int)positions[num].pos.y, (float)positions[num].pos.th,
		(int)positions[num].ball.x, (int)positions[num].ball.y, (char *)comm_info.command);
}

void Interface::paintEvent(QPaintEvent *e)
{
}

void Interface::receiveStateChange(int state)
{
	if(state == Qt::Checked) {
		/* start*/
		for(int i = 0; i < robot_num; i++)
			th[i]->start();
	} else {
		/* stop */
		for(int i = 0; i < robot_num; i++)
			th[i]->quit();
	}
}

void Interface::reverseField(int state)
{
	if(state == Qt::Checked)
		fReverse = true;
	else
		fReverse = false;
}

