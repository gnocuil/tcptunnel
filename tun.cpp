#include <linux/if_tun.h>
#include <net/if.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <iostream>
#include <netinet/ip6.h>  
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

#include "tun.h"
#include "socket.h"
#include "network.h"

static int tun_fd;
static char buf[2040];

struct in_addr addr_local, addr_remote;
unsigned short port_local, port_remote;

int tun_create(char *dev)
{
	struct ifreq ifr;
	int err;

	if ((tun_fd = open("/dev/net/tun", O_RDWR)) < 0) {
		fprintf(stderr, "tun_create: Error Creating TUN/TAP: %m\n", errno);
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags |= IFF_TUN | IFF_NO_PI;

	if (*dev != '\0') {
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	if ((err = ioctl(tun_fd, TUNSETIFF, (void *)&ifr)) < 0) {
		fprintf(stderr, "tun_create: Error Setting tunnel name %s: %m\n", dev, errno);
		close(tun_fd);
		return -1;
	}

	if (fcntl(tun_fd, F_SETFL, O_NONBLOCK) < 0) {
		fprintf(stderr, "tun_create: Error Setting nonblock: %m\n", dev, errno);
		return -1;
	}

	
	strcpy(dev, ifr.ifr_name);
	
	return tun_fd;
}

int tun_send(char *packet, int len)
{
	int count = write(tun_fd, packet, len);
	if (count != len) {
		fprintf(stderr, "tun_send : Error sending len=%d count=%d: %m\n", len, count, errno);
		return -1;
	}
	return 0;
}

static uint16_t bigpacket[65536];
static uint16_t getport_dest(char *ippacket)
{
	struct iphdr *iph = (struct iphdr*)ippacket;
	uint16_t ret = 0;
	uint8_t flags = (uint8_t)(ntohs(iph->frag_off) >> 13);
	//fragment offset
	uint16_t frag_off = ntohs(iph->frag_off) & 0x1FFF;
	
	//for fragmented ipv4 packets, choose the port number in the first packet
	if (frag_off != 0)
		return bigpacket[iph->id];
	
	int protoff = iph -> ihl * 4;
	if (iph->protocol == IPPROTO_TCP || iph->protocol == IPPROTO_UDP) {
		struct tcphdr* tcph = (struct tcphdr*)(ippacket + protoff);
		ret = ntohs(tcph->dest);
	} else if (iph->protocol == IPPROTO_ICMP) {
		struct icmp *icmph = (struct icmp*)(ippacket + protoff);
		switch (icmph->icmp_type) {
			case ICMP_ECHOREPLY:
			case ICMP_ECHO:
				ret = htons(icmph->icmp_id);
				break;
			default:
				iph = (struct iphdr*) (((uint8_t*)icmph) + 8);
				protoff = iph -> ihl * 4;
				if (iph->protocol == IPPROTO_TCP || iph->protocol == IPPROTO_UDP) {
					struct tcphdr* tcph = (struct tcphdr*) (iph + protoff);
					ret = ntohs(tcph->source);
				}
				break;
		}
	} 
	if ((flags & 1) && frag_off == 0)
		bigpacket[iph->id] = ret;
	return ret;
}

int handle_tun()
{
	int len = read(tun_fd, encap->readbuf(), encap->readbuflen());
	if (len < 0)
		return -1;

	encap->makepacket(len);

	return socket_send(encap->sendbuf(), encap->sendlen());
	//return tun_send(buf, len + 40);
}
