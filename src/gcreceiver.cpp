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
		game_state.setData(buf, datagrams.size());
		emit remainingTimeChanged(game_state.getRemainingTime());
		if(game_state.updatedScore1()) {
			emit scoreChanged1(game_state.getScore1());
		}
		if(game_state.updatedScore2()) {
			emit scoreChanged2(game_state.getScore2());
		}
		delete[] buf;
	}
}

