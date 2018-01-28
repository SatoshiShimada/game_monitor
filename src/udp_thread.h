#ifndef _UDP_THREAD_H_
#define _UDP_THREAD_H_

#include <QtGui>
#include <QUdpSocket>
#include <QtCore>

#include "pos_types.h"

Q_DECLARE_METATYPE(comm_info_T);

class UdpServer : public QObject
{
	Q_OBJECT
public:
	UdpServer(int, int);
	~UdpServer();
private:
	struct comm_info_T _comm_info;
	int _id;
	QUdpSocket *_udpSocket;
private slots:
	void readPendingDatagrams(void);
signals:
	void receiveData(int, struct comm_info_T);
};

bool getCommInfoObject(unsigned char *, Object *);

#endif // _UDP_THREAD_H_

