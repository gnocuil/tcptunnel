#pragma once

#include <netinet/ip.h>  
#include <algorithm>

#include "encap.h"

class Encap_UDP : public Encap {
public:
	const char* name() { return "IP-in-UDP"; }
	char* readbuf() {
		return buf;
	}
	int readbuflen() {
		return BUF_LEN;
	}
	char* sendbuf() {
		return buf;
	}
	int makepacket(int len) {
		std::reverse(buf, buf + len);
		send_len = len;
		return 0;
	}
	int init_socket() {
		raw_fd = socket(AF_INET, SOCK_DGRAM, 0);
		struct sockaddr_in servaddr;
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(port_local);
		bind(raw_fd, (struct sockaddr *)&servaddr,sizeof(servaddr));
		return raw_fd;
	}
	int handle_socket() {
		struct sockaddr_in6 sin6addr;
		socklen_t addr_len = sizeof (sin6addr);
		int len = recvfrom(raw_fd, buf4, BUF_LEN, 0, 0, 0);
		if (len < 0)
			return -1;
//make
		std::reverse(buf4, buf4 + len);

		send4_len = len;
		return 0;
	}
	char* send4buf() {
		return buf4;
	}
private:
	char buf[BUF_LEN];
	char buf4[BUF_LEN];
	int raw_fd;
};
