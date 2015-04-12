#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>  
#include <netinet/udp.h> 
#include <string.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>

#include "socket.h"
#include "tun.h"
#include "network.h"

//static int raw_fd;

static int udp_fd;
static char buf[2000];

int socket_init()
{
	int raw_fd = encap->init_socket();
	if (raw_fd < 0)
		return -1;
	//raw_fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));
	//raw_fd = socket(AF_INET6, SOCK_RAW, IPPROTO_IPIP);
	
//	if (raw_fd < 0) {
//		fprintf(stderr, "socket_init: Error Creating socket: %m\n", errno);
//		return -1;
//	}
	
//	if (fcntl(raw_fd, F_SETFL, O_NONBLOCK) < 0) {i
//		fprintf(stderr, "socket_init: Error Setting nonblock: %m\n", errno);
//		return -1;
//	}
/*	
	send4_fd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
	if (send4_fd < 0) {
		fprintf(stderr, "socket_init : Error Creating send4 socket: %m\n", errno);
		return -1;
	}
*/
	return raw_fd;
}

int socket_init_tun()
{
/*
	send6_fd = socket(PF_INET6, SOCK_RAW, IPPROTO_RAW);
	if (send6_fd < 0) {
		fprintf(stderr, "socket_init: Error Creating send socket: %m\n", errno);
		return -1;
	}
*/
	udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
}

int handle_socket()
{
	if (encap->handle_socket() < 0)
		return -1;

	//sin6addr.sin6_addr is the IPv6 addr of TI (struct in6_addr)
	//socket_send4(buf, len);
	tun_send(encap->send4buf(), encap->send4len());
}

int socket_send(char *buf, int len)
{
/*
	struct sockaddr_in6 dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin6_family = AF_INET6;
	memcpy(&dest.sin6_addr, buf + 24, 16);
	
	if (sendto(send6_fd, buf, len, 0, (struct sockaddr *)&dest, sizeof(dest)) != len) {
		fprintf(stderr, "socket_send: Failed to send ipv6 packet len=%d: %m\n", len, errno);
		//for (int i = 0; i < len; ++i) printf("%d:%x ", i + 1, buf[i] & 0xFF);printf("\n");
		return -1;
	}
	return 0;
*/
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = addr_remote;
	servaddr.sin_port = htons(port_remote);

	static int init = 0;
	if (!init) {
		init = 1;
		connect(udp_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	}


	sendto(udp_fd, buf, len, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
}
