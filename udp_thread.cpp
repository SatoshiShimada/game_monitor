
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <boost/math/constants/constants.hpp>

#include "udp_thread.h"

UdpThread::UdpThread(int port_num)
{
	struct addrinfo hints, *res;
	int err;
	char port[20];

	sprintf(port, "%d", port_num);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	err = getaddrinfo(NULL, port, &hints, &res);
	if(err != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		//return 1;
	}
	sock = socket(res->ai_family, res->ai_socktype, 0);
	if(sock < 0) {
		perror("socket");
		//return 1;
	}
	if(bind(sock, res->ai_addr, res->ai_addrlen) != 0) {
		perror("bind");
		//return 1;
	}
	freeaddrinfo(res);
}

void UdpThread::run(void)
{
	unsigned char buf[2048];
	size_t n;

	while(1) {
		// receive
		memset(buf, 0, sizeof(buf));
		/* n is sizeof received data */
		n = (size_t)recv(sock, buf, sizeof(buf), 0);

		// decode
		char *p = (char *)&comm_info;
		for(size_t i = 0; (i < n) && (i < sizeof(struct comm_info_T)); i++) {
			*p++ = buf[i];
		}
		/* [ DEBUG ]
		unsigned int color;
		unsigned int id;
		color = (buf[0] & 0x80) >> 7;
		id    = buf[0] & 0x7F;
		if(color == MAGENTA)
			fprintf(stderr, "[MAGENTA] [ID: %d]\n", id);
		else if(color == CYAN)
			fprintf(stderr, "[CYAN] [ID: %d]\n", id);
		fprintf(stderr, "comm_info.status: %d\n", comm_info.status);
		fprintf(stderr, "comm_info.fps: %d\n", comm_info.fps);
		fprintf(stderr, "comm_info.voltage: %f[V]\n", (comm_info.voltage << 3) / 100.0);
		fprintf(stderr, "comm_info.command: %s\n", comm_info.command);
		fprintf(stderr, "-----------------\n");
		*/

		emit receiveData(comm_info);
	}

	return;
}

UdpThread::~UdpThread()
{
	close(sock);
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

