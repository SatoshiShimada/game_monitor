
#include <iostream>
#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QTextEdit> /* multi line */
#include <QLineEdit> /* single line */
#include <QMainWindow>
#include <QDropEvent>
#include <QDrag>
#include <QUrl>
#include <QMimeData>
#include <QString>
#include <QMetaType>
#include <QPainter>

#include "udp_thread.h"
#include "hpl_types.h"

struct robot {
	QLabel *name;
	QLabel *voltage;
	QLabel *fps;
	QLabel *string;
	Pos2D pos;
};

class Interface : public QMainWindow
{
	Q_OBJECT

private:
	UdpThread *th1;
	UdpThread *th2;
	UdpThread *th3;
	UdpThread *th4;
	UdpThread *th5;
	QCheckBox *receive, *reverse;
	QPushButton *logStart, *logTerm;
	QLabel *id, *name, *voltage, *fps, *string;
	QLabel *idLabel1, *idLabel2, *idLabel3, *idLabel4, *idLabel5;
	QString filenameDrag;
	QWidget *window;
	struct robot robot1, robot2, robot3, robot4, robot5;
	QLabel *titleLabel;
	QLabel *image;
	QPixmap map;
	QPixmap origin_map;
	QVBoxLayout *mainLayout;
	QVBoxLayout *buttonLayout;
	QHBoxLayout *checkLayout;
	QHBoxLayout *winLayout;
	QGridLayout *labelLayout;
	void connection(void);
	bool fLogging;
	bool fReceive;
	bool fReverse;

protected:
	void paintEvent(QPaintEvent *);

public:
	Interface();
	~Interface();
	void createWindow(void);
	void loadImage(const char *);
	void loadImage(QString);
	void dragEnterEvent(QDragEnterEvent *);
	void dropEvent(QDropEvent *);
	void decodeUdp(struct comm_info_T, struct robot *);

private slots:
	void decodeData1(struct comm_info_T);
	void decodeData2(struct comm_info_T);
	void decodeData3(struct comm_info_T);
	void decodeData4(struct comm_info_T);
	void decodeData5(struct comm_info_T);
};

