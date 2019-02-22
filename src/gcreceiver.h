#ifndef GCRECEIVER_H
#define GCRECEIVER_H

#include <QtGui>
#include <QUdpSocket>
#include <QtCore>

#include "game_state.h"

class GCReceiver : public QObject
{
	Q_OBJECT
public:
	GCReceiver(const int);
	~GCReceiver();
private:
	QUdpSocket *udpSocket;
	GameState gc_data;
signals:
	void gameStateChanged(int);
	void remainingTimeChanged(int);
	void secondaryTimeChanged(int);
	void scoreChanged1(int);
	void scoreChanged2(int);
private slots:
	void readPendingDatagrams(void);
};

#endif // GCRECEIVER_H

