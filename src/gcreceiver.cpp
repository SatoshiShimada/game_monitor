#include "gcreceiver.h"

GCReceiver::GCReceiver(int port_num)
{
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(QHostAddress::Any, port_num);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

GCReceiver::~GCReceiver()
{
}

void GCReceiver::readPendingDatagrams(void)
{
	while(udpSocket->hasPendingDatagrams()) {
		QByteArray datagrams;
		datagrams.resize(udpSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;
		udpSocket->readDatagram(datagrams.data(), datagrams.size(), &sender, &senderPort);
		char *p = datagrams.data();
		char *buf = new char[datagrams.size()];
		for(size_t i = 0; (i < (size_t)datagrams.size()); i++) {
			buf[i] = p[i];
		}
		gc_data.setData(buf, datagrams.size());
		emit gameStateChanged(gc_data.getGameState());
		emit remainingTimeChanged(gc_data.getRemainingTime());
		emit secondaryTimeChanged(gc_data.getSecondaryTime());
		if(gc_data.updatedScore1()) {
			emit scoreChanged1(gc_data.getScore1());
		}
		if(gc_data.updatedScore2()) {
			emit scoreChanged2(gc_data.getScore2());
		}
		delete[] buf;
	}
}

