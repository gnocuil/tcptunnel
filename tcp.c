#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string.h>
#include "tcp.h"
#include "defines.h"

int fd_c2s;
int fd_s2c;

struct in_addr addr_remote;

static int init_fd(uint16_t port)
{
    int fd;
    if ((fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "binding_init: Failed to create socket: %m\n");
        exit(1);
    }
    int no = 0;
    int yes = 1;
    setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    
    struct sockaddr_in6 serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(port);
    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)  {
        perror("error in bind()");
        exit(0);
    }

    if (listen(fd, 1000) != 0)  {
        perror("error in listen()");
        exit(0);
    }
}

void tcpsrv_init()
{
    fd_c2s = init_fd(PORT_C2S);
    fd_s2c = init_fd(PORT_S2C);
}

static int connect_fd(uint16_t port)
{
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
		fprintf(stderr, "init_fd: failed to create socket: %m\n");
		return -1;
	}
    struct sockaddr_in serv_addr;
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = addr_remote;
    if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(0);
    }
    return fd;
}

void tcpcli_init()
{
    fd_c2s = connect_fd(PORT_C2S);
}

