
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <thread>
#include <exception>
#include <boost/asio.hpp>
#include <boost/math/constants/constants.hpp>

#include "udp_thread.h"

UdpThread::UdpThread(int port_num)
{
	try {
		socket = new boost::asio::ip::udp::socket(io_srv, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port_num));
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void UdpThread::run(void)
{
	unsigned char buf[2048];
	size_t len;

	while(1) {
		len = 0;
		memset(buf, 0, sizeof(buf));
		// receive data
		try {
			len = socket->receive_from(boost::asio::buffer(buf, sizeof(buf)), remote_endpoint, 0, error);
		} catch(std::exception &e) {
			std::cerr << e.what() << std::endl;
		}

		// copy receivied data to structure
		char *p = (char *)&comm_info;
		for(size_t i = 0; (i < len) && (i < sizeof(struct comm_info_T)); i++) {
			*p++ = buf[i];
		}

		emit receiveData(comm_info);
	}
	return;
}

UdpThread::~UdpThread()
{
}

bool getCommInfoObject(unsigned char *data, Pos2D *pos)
{
	int x, y, theta;
	bool res;
	bool is_our_side, is_opposite_side;
	float th;

	/* calculate position of object */
	if(data[0] & COMM_EXIST) {
		res = true;
		x = ((data[0] & 0x0f) << 6) + ((data[1] & 0xfc) >> 2);
		if((x & 0x0200) != 0) x = -(0x0400 - x);
		x *= 10;
		y = ((data[1] & 0x03) << 8) + data[2];
		if((y & 0x200) != 0) y = -(0x0400 - y);
		y *= 10;
		theta = (data[3] * 2) - 180;
		th = (float)(theta * boost::math::constants::pi<float>() / 180.0f);
		is_our_side = ((data[0] & COMM_OUR_SIDE) != 0) ? true : false;
		is_opposite_side = ((data[0] & COMM_OPPOSITE_SIDE) != 0) ? true : false;
		pos->x = x;
		pos->y = y;
		pos->th = th;
	} else {
		res = false;
	}
	return res;
}

