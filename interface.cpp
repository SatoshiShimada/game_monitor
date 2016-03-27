
#include "hpl_types.h"

#include <QtGui>
#include "interface.h"
#include <iostream>
#include <string.h>

Interface::Interface()
{
	qRegisterMetaType<comm_info_T>("comm_info_T");
	setAcceptDrops(true);
	/* Initialize flags */
	fLogging = true;
	fReceive = true;
	fReverse = false;

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
	th1            = new UdpThread(7110);
	th2            = new UdpThread(7111);
	th3            = new UdpThread(7112);
	th4            = new UdpThread(7113);
	th5            = new UdpThread(7114);
	th6            = new UdpThread(7115);
	receive        = new QCheckBox("Receive data");
	reverse        = new QCheckBox("Reverse field");
	logStart       = new QPushButton("Log start");
	logTerm        = new QPushButton("Log terminate");
	image          = new QLabel;
	id             = new QLabel("ID");
	name           = new QLabel("Name");
	voltage        = new QLabel("Voltage");
	fps            = new QLabel("FPS");
	string         = new QLabel("Common String");
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
	mainLayout     = new QVBoxLayout;
	buttonLayout   = new QVBoxLayout;
	checkLayout    = new QHBoxLayout;
	winLayout      = new QHBoxLayout;
	labelLayout    = new QGridLayout;

	/* set color for label */
	pal_red = robot1.string->palette();
	pal_green = robot1.string->palette();
	pal_blue = robot1.string->palette();
	pal_black = robot1.string->palette();
	pal_orange = robot1.string->palette();
	pal_red.setColor(QPalette::Foreground, QColor("#FF0000"));
	pal_green.setColor(QPalette::Foreground, QColor("#00FF00"));
	pal_blue.setColor(QPalette::Foreground, QColor("#0000FF"));
	pal_black.setColor(QPalette::Foreground, QColor("#000000"));
	pal_orange.setColor(QPalette::Foreground, QColor("#FFA500"));
	robot1.string->setPalette(pal_black);
	robot2.string->setPalette(pal_black);
	robot3.string->setPalette(pal_black);
	robot4.string->setPalette(pal_black);
	robot5.string->setPalette(pal_black);
	robot6.string->setPalette(pal_black);

	logStart->setEnabled(false);

	//image->setMinimumSize(280, 280);
	//image->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
	loadImage("hlfield.png");

	labelLayout->addWidget(id,      1, 1);
	labelLayout->addWidget(name,    1, 2);
	labelLayout->addWidget(voltage, 1, 3);
	labelLayout->addWidget(fps,     1, 4);
	labelLayout->addWidget(string,  1, 5);

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

	buttonLayout->addWidget(logStart);
	buttonLayout->addWidget(logTerm);

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
		std::cerr << "Error: can\'t open image file" << std::endl;
		return;
	}
	origin_map = QPixmap::fromImage(image_buf);
	map = origin_map;
	//map = map.scaled(280, 280);
	image->setPixmap(map);
}

void Interface::loadImage(const char *image_filename)
{
	QImage image_buf(image_filename);
	if(image_buf.isNull()) {
		std::cerr << "Error: can\'t open image file" << std::endl;
		return;
	}
	origin_map = QPixmap::fromImage(image_buf);
	map = origin_map;
	//map = map.scaled(280, 280);
	image->setPixmap(QPixmap::fromImage(image_buf));
}

void Interface::dragEnterEvent(QDragEnterEvent *e)
{
	if(e->mimeData()->hasFormat("text/uri-list"))
	{
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
	connect(th6, SIGNAL(receiveData(struct comm_info_T)), this, SLOT(decodeData5(struct comm_info_T)));
}

void Interface::decodeData1(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot1);
}

void Interface::decodeData2(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot2);
}

void Interface::decodeData3(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot3);
}

void Interface::decodeData4(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot4);
}

void Interface::decodeData5(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot5);
}

void Interface::decodeData6(struct comm_info_T comm_info)
{
	decodeUdp(comm_info, &robot6);
}

void Interface::decodeUdp(struct comm_info_T comm_info, struct robot *robot_data)
{
	char buf[2048];
	int color, id;

	/* MAGENTA, CYAN */
	color = (int)(comm_info.id & 0x80) >> 7;
	id    = (int)(comm_info.id & 0x7F);

	sprintf(buf, "%s %d", ((color == MAGENTA) ? "MAGENTA" : "CYAN"), id);
	robot_data->name->setText(buf);
	sprintf(buf, "%lf", (comm_info.voltage << 3) / 100.0);
	robot_data->voltage->setText(buf);
	sprintf(buf, "%d", comm_info.fps);
	robot_data->fps->setText(buf);
	if(strstr((const char *)comm_info.command, "Attacker")) {
		robot_data->string->setPalette(pal_red);
		/* Red */
	} else if(strstr((const char *)comm_info.command, "Neutral")) {
		robot_data->string->setPalette(pal_green);
		/* Green */
	} else if(strstr((const char *)comm_info.command, "Defecder")) {
		robot_data->string->setPalette(pal_blue);
		/* Blue */
	} else if(strstr((const char *)comm_info.command, "Keeper")) {
		robot_data->string->setPalette(pal_orange);
		/* Orange */
	} else {
		robot_data->string->setPalette(pal_black);
		/* Black */
	}
	robot_data->string->setText((char *)comm_info.command);
	/* erase previous position marker */
	map = origin_map;
	QPainter paint(&map);
	QPen ppen(Qt::blue);
	ppen.setWidth(10);
	paint.setPen(ppen);
	paint.setBrush(Qt::blue);

	getCommInfoObject(comm_info.object[0], &(robot_data->pos));
	// 20: scale(if teen league: 1.5times)
	robot_data->pos.x = (int)( robot_data->pos.x / 20) + (370 / 2);
	robot_data->pos.y = (int)(-robot_data->pos.y / 20) + (270 / 2);
	paint.drawPoint(robot_data->pos.x, robot_data->pos.y);
	image->setPixmap(map);
}

void Interface::paintEvent(QPaintEvent *e)
{
}

