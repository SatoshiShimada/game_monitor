
#include <QtGui>
#include <iostream>
#include <cstring>
#include <cmath>

#include "pos_types.h"
#include "interface.h"

Interface::Interface()
{
	qRegisterMetaType<comm_info_T>("comm_info_T");
	setAcceptDrops(true);
	log.setEnable();

	settings = new QSettings("./config.ini", QSettings::IniFormat);
	/* 370x270 pixel: field image size */
	settings->setValue("field_image_width" , 740);
	settings->setValue("field_image_height", 540);
	/* field size 10000x7000 milli meter? (map size in robot used) */
	settings->setValue("field_size_x", 10000);
	settings->setValue("field_size_y", 7000);
	/* marker */
	settings->setValue("robot_marker_size", 5*2);
	settings->setValue("ball_marker_size", 3*2);
	settings->setValue("theta_length", 8*2);

	/* Initialize flags */
	fLogging = true;
	fReceive = true;
	fReverse = false;

	/* using UDP communication port offset */
	port = 7110;
	createWindow();
	connection();

	/* Run thread */
	th1->start();
	th2->start();
	th3->start();
	th4->start();
	th5->start();
	th6->start();

	this->setWindowTitle("Humanoid League Game Monitor");
	this->resize(840, 320);
}

Interface::~Interface()
{
}

void Interface::createWindow(void)
{
	window         = new QWidget;
	titleLabel     = new QLabel("Humanoid League Game Monitor");
	th1            = new UdpThread(port + 0);
	th2            = new UdpThread(port + 1);
	th3            = new UdpThread(port + 2);
	th4            = new UdpThread(port + 3);
	th5            = new UdpThread(port + 4);
	th6            = new UdpThread(port + 5);
	receive        = new QCheckBox("Receive data");
	reverse        = new QCheckBox("Reverse field");
	image          = new QLabel;
	id             = new QLabel("ID");
	name           = new QLabel("Name");
	voltage        = new QLabel("Voltage");
	fps            = new QLabel("FPS");
	string         = new QLabel("Common String");
	cf_own         = new QLabel("Reliability of self pos");
	cf_ball        = new QLabel("Reliability of ball pos");
	idLabel1       = new QLabel("1");
	idLabel2       = new QLabel("2");
	idLabel3       = new QLabel("3");
	idLabel4       = new QLabel("4");
	idLabel5       = new QLabel("5");
	idLabel6       = new QLabel("6");
	robot1.name    = new QLabel("");
	robot2.name    = new QLabel("");
	robot3.name    = new QLabel("");
	robot4.name    = new QLabel("");
	robot5.name    = new QLabel("");
	robot6.name    = new QLabel("");
	robot1.voltage = new QLabel("");
	robot2.voltage = new QLabel("");
	robot3.voltage = new QLabel("");
	robot4.voltage = new QLabel("");
	robot5.voltage = new QLabel("");
	robot6.voltage = new QLabel("");
	robot1.fps     = new QLabel("");
	robot2.fps     = new QLabel("");
	robot3.fps     = new QLabel("");
	robot4.fps     = new QLabel("");
	robot5.fps     = new QLabel("");
	robot6.fps     = new QLabel("");
	robot1.string  = new QLabel("");
	robot2.string  = new QLabel("");
	robot3.string  = new QLabel("");
	robot4.string  = new QLabel("");
	robot5.string  = new QLabel("");
	robot6.string  = new QLabel("");
	robot1.cf_own  = new QLabel("");
	robot2.cf_own  = new QLabel("");
	robot3.cf_own  = new QLabel("");
	robot4.cf_own  = new QLabel("");
	robot5.cf_own  = new QLabel("");
	robot6.cf_own  = new QLabel("");
	robot1.cf_ball = new QLabel("");
	robot2.cf_ball = new QLabel("");
	robot3.cf_ball = new QLabel("");
	robot4.cf_ball = new QLabel("");
	robot5.cf_ball = new QLabel("");
	robot6.cf_ball = new QLabel("");
	mainLayout     = new QVBoxLayout;
	buttonLayout   = new QVBoxLayout;
	checkLayout    = new QHBoxLayout;
	winLayout      = new QHBoxLayout;
	labelLayout    = new QGridLayout;

	/* set width */
	id     ->setFixedWidth(30);
	name   ->setFixedWidth(100);
	voltage->setFixedWidth(60);
	fps    ->setFixedWidth(30);
	cf_own ->setFixedWidth(120);
	cf_ball->setFixedWidth(120);

	/* set color palette for label */
	pal_red    = robot1.string->palette();
	pal_green  = robot1.string->palette();
	pal_blue   = robot1.string->palette();
	pal_black  = robot1.string->palette();
	pal_orange = robot1.string->palette();
	pal_red.   setColor(QPalette::Foreground, QColor("#FF0000"));
	pal_green. setColor(QPalette::Foreground, QColor("#00FF00"));
	pal_blue.  setColor(QPalette::Foreground, QColor("#0000FF"));
	pal_black. setColor(QPalette::Foreground, QColor("#000000"));
	pal_orange.setColor(QPalette::Foreground, QColor("#FFA500"));
	/* all robot default color is black */
	robot1.string->setPalette(pal_black);
	robot2.string->setPalette(pal_black);
	robot3.string->setPalette(pal_black);
	robot4.string->setPalette(pal_black);
	robot5.string->setPalette(pal_black);
	robot6.string->setPalette(pal_black);

	loadImage("hlfield.png");

	labelLayout->addWidget(id,      1, 1);
	labelLayout->addWidget(name,    1, 2);
	labelLayout->addWidget(voltage, 1, 3);
	labelLayout->addWidget(fps,     1, 4);
	labelLayout->addWidget(string,  1, 5);
	labelLayout->addWidget(cf_own,  1, 6);
	labelLayout->addWidget(cf_ball, 1, 7);

	labelLayout->addWidget(idLabel1, 2, 1);
	labelLayout->addWidget(idLabel2, 3, 1);
	labelLayout->addWidget(idLabel3, 4, 1);
	labelLayout->addWidget(idLabel4, 5, 1);
	labelLayout->addWidget(idLabel5, 6, 1);
	labelLayout->addWidget(idLabel6, 7, 1);
	labelLayout->addWidget(robot1.name, 2, 2);
	labelLayout->addWidget(robot2.name, 3, 2);
	labelLayout->addWidget(robot3.name, 4, 2);
	labelLayout->addWidget(robot4.name, 5, 2);
	labelLayout->addWidget(robot5.name, 6, 2);
	labelLayout->addWidget(robot6.name, 7, 2);
	labelLayout->addWidget(robot1.voltage, 2, 3);
	labelLayout->addWidget(robot2.voltage, 3, 3);
	labelLayout->addWidget(robot3.voltage, 4, 3);
	labelLayout->addWidget(robot4.voltage, 5, 3);
	labelLayout->addWidget(robot5.voltage, 6, 3);
	labelLayout->addWidget(robot6.voltage, 7, 3);
	labelLayout->addWidget(robot1.fps, 2, 4);
	labelLayout->addWidget(robot2.fps, 3, 4);
	labelLayout->addWidget(robot3.fps, 4, 4);
	labelLayout->addWidget(robot4.fps, 5, 4);
	labelLayout->addWidget(robot5.fps, 6, 4);
	labelLayout->addWidget(robot6.fps, 7, 4);
	labelLayout->addWidget(robot1.string, 2, 5);
	labelLayout->addWidget(robot2.string, 3, 5);
	labelLayout->addWidget(robot3.string, 4, 5);
	labelLayout->addWidget(robot4.string, 5, 5);
	labelLayout->addWidget(robot5.string, 6, 5);
	labelLayout->addWidget(robot6.string, 7, 5);
	labelLayout->addWidget(robot1.cf_own, 2, 6);
	labelLayout->addWidget(robot2.cf_own, 3, 6);
	labelLayout->addWidget(robot3.cf_own, 4, 6);
	labelLayout->addWidget(robot4.cf_own, 5, 6);
	labelLayout->addWidget(robot5.cf_own, 6, 6);
	labelLayout->addWidget(robot6.cf_own, 7, 6);
	labelLayout->addWidget(robot1.cf_own, 2, 7);
	labelLayout->addWidget(robot2.cf_own, 3, 7);
	labelLayout->addWidget(robot3.cf_own, 4, 7);
	labelLayout->addWidget(robot4.cf_own, 5, 7);
	labelLayout->addWidget(robot5.cf_own, 6, 7);
	labelLayout->addWidget(robot6.cf_own, 7, 7);

	checkLayout->addWidget(receive);
	checkLayout->addWidget(reverse);

	winLayout->addWidget(image);
	winLayout->addLayout(buttonLayout);

	mainLayout->addWidget(titleLabel);
	mainLayout->addLayout(checkLayout);
	mainLayout->addLayout(winLayout);
	mainLayout->addLayout(labelLayout);

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
	connect(th1, SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData1(struct comm_info_T)));
	connect(th2, SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData2(struct comm_info_T)));
	connect(th3, SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData3(struct comm_info_T)));
	connect(th4, SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData4(struct comm_info_T)));
	connect(th5, SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData5(struct comm_info_T)));
	connect(th6, SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData6(struct comm_info_T)));
	connect(receive, SIGNAL(toggled(bool)), this, SLOT(receiveStateChange(bool)));
	connect(reverse, SIGNAL(toggled(bool)), this, SLOT(reverseField(bool)));
}

void Interface::decodeData1(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot1, 1);
}

void Interface::decodeData2(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot2, 2);
}

void Interface::decodeData3(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot3, 3);
}

void Interface::decodeData4(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot4, 4);
}

void Interface::decodeData5(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot5, 5);
}

void Interface::decodeData6(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot6, 6);
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
	sprintf(buf, "%d", comm_info.fps);
	robot_data->fps->setText(buf);
	/* Self-position confidence */
	sprintf(buf, "%d", comm_info.cf_own);
	robot_data->cf_own->setText(buf);
	/* Ball position confidence */
	sprintf(buf, "%d", comm_info.cf_ball);
	robot_data->cf_ball->setText(buf);
	/* Role and message */
	if(strstr((const char *)comm_info.command, "Attacker")) {
		/* Red */
		robot_data->string->setPalette(pal_red);
		strcpy(positions[num].color, "red");
	} else if(strstr((const char *)comm_info.command, "Neutral")) {
		/* Green */
		robot_data->string->setPalette(pal_green);
		strcpy(positions[num].color, "green");
	} else if(strstr((const char *)comm_info.command, "Defender")) {
		/* Blue */
		robot_data->string->setPalette(pal_blue);
		strcpy(positions[num].color, "blue");
	} else if(strstr((const char *)comm_info.command, "Keeper")) {
		/* Orange */
		robot_data->string->setPalette(pal_orange);
		strcpy(positions[num].color, "orange");
	} else {
		/* Black */
		robot_data->string->setPalette(pal_black);
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
				paint.setPen(QPen(QColor(0xFF, 0x00, 0x00), settings->value("robot_marker_size").toInt()));
			} else {
				paint.setPen(QPen(QColor(0x00, 0x00, 0x00), settings->value("robot_marker_size").toInt()));
			}
			/* draw robot position */
			paint.drawPoint(self_x, self_y);
			/* calclate robot theta */
			double direction_x = self_x + settings->value("theta_length").toInt() * cos(positions[i].pos.th);
			double direction_y = self_y + settings->value("theta_length").toInt() * sin(positions[i].pos.th);
			paint.drawLine(self_x, self_y, direction_x, direction_y);
			sprintf(buf, "%d", i);
			paint.drawText(QPoint(self_x, self_y), buf);
			if(positions[i].enable_ball == true) {
				/* draw ball position as orange */
				paint.setPen(QPen(QColor(0xFF, 0xA5, 0x00), settings->value("ball_marker_size").toInt()));
				paint.drawPoint(ball_x, ball_y);
				sprintf(buf, "%d", i);
				paint.drawText(QPoint(ball_x, ball_y), buf);
			}
		}
		positions[i].lastReceive++;
	}
	image->setPixmap(map);
	log.write(num, color_str, (int)comm_info.fps, (double)voltage,
		(int)positions[num].pos.x, (int)positions[num].pos.y, (float)positions[num].pos.th,
		(int)positions[num].ball.x, (int)positions[num].ball.y, (char *)comm_info.command);
}

void Interface::paintEvent(QPaintEvent *e)
{
}

void Interface::receiveStateChange(bool checked)
{
	if(receive->isTristate()) {
		/* start*/
		th1->start();
		th2->start();
		th3->start();
		th4->start();
		th5->start();
		th6->start();
	} else {
		/* stop */
		th1->quit();
		th2->quit();
		th3->quit();
		th4->quit();
		th5->quit();
		th6->quit();
	}
}

void Interface::reverseField(bool cheched)
{
	if(fReverse)
		fReverse = false;
	else
		fReverse = true;
}

