#include <cmath>

#include "udp_thread.h"

inline static double radian(double degree)
{
	return degree * M_PI / 180.0;
}

UdpServer::UdpServer(int id, int port_num) _id(id)
{
	_udpSocket = new QUdpSocket(this);
	_udpSocket->bind(QHostAddress::Any, port_num);
	connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void UdpServer::readPendingDatagrams(void)
{
	while(_udpSocket->hasPendingDatagrams()) {
		QByteArray datagrams;
		datagrams.resize(_udpSocket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;
		_udpSocket->readDatagram(datagrams.data(), datagrams.size(), &sender, &senderPort);
		char *buf = datagrams.data();
		char *p = (char *)&_comm_info;
		for(size_t i = 0; (i < (unsigned)datagrams.size()) && (i < sizeof(struct comm_info_T)); i++) {
			*p++ = buf[i];
		}
		emit receiveData(_id, _comm_info);
	}
}

UdpServer::~UdpServer()
{
}

