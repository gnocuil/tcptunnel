#include <stdio.h>
#include <stdlib.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h> 
#include <net/if.h>  
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "tun.h"
#include "defines.h"

int tun_fd;

int tun_init()
{
	struct ifreq ifr;
	int err;

	if ((tun_fd = open("/dev/net/tun", O_RDWR)) < 0) {
		fprintf(stderr, "tun_create: Error Creating TUN/TAP: %m\n");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags |= IFF_TUN | IFF_NO_PI;

	strncpy(ifr.ifr_name, TUNNEL_NAME, IFNAMSIZ);


	if ((err = ioctl(tun_fd, TUNSETIFF, (void *)&ifr)) < 0) {
		fprintf(stderr, "tun_create: Error Setting tunnel name %s: %m\n", TUNNEL_NAME);
		close(tun_fd);
		return -1;
	}
/*
	if (fcntl(tun_fd, F_SETFL, O_NONBLOCK) < 0) {
		fprintf(stderr, "tun_create: Error Setting nonblock: %m\n");
		return -1;
	}
*/
	return tun_fd;
}


int tun_send(char *packet, int len)
{
	int count = write(tun_fd, packet, len);
	if (count != len) {
		fprintf(stderr, "tun_send : Error sending len=%d count=%d: %m\n", len, count);
		return -1;
	}
	return 0;
}

int handle_tun(char* buf, int buflen)
{
	int len = read(tun_fd, buf, buflen);
	if (len < 0)
		return len;

	return len;
}

