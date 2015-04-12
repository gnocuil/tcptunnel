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

//int fdlist_s2c[LIST_MAX];
//int fdlist_c2s[LIST_MAX];
//int fdlist_s2c_len;
//int fdlist_c2s_len;

int fd_data_c2s;
int fd_data_s2c;


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
    return fd;
}

void tcpsrv_init()
{
    fd_c2s = init_fd(PORT_C2S);
    fd_s2c = init_fd(PORT_S2C);
}

void handle_c2s()
{
    int fd = accept(fd_c2s, NULL, NULL);
//    if (fdlist_c2s_len < LIST_MAX) {
//        fdlist_c2s[fdlist_c2s_len++] = fd;
//    }
    fd_data_c2s = fd;
}

void handle_s2c()
{
    int fd = accept(fd_s2c, NULL, NULL);
//    if (fdlist_s2c_len < LIST_MAX) {
//        fdlist_s2c[fdlist_s2c_len++] = fd;
//    }
    fd_data_s2c = fd;
}

int handle_socket(int fd, char *buf)
{
    uint16_t x;
    int count = read(fd, &x, sizeof(x));
    printf("received cnt=%d x=%d\n", count, x);
    if (count <= 0) return count;
    count = read(fd, buf, (int)x);
    if (count != (int)x)
        fprintf(stderr, "handle_socket: read %d, but want %d\n", count, (int)x);
    return count;
}

int socket_send(int fd, char* buf, int len)
{
    if (fd > 0) {
        uint16_t x = (uint16_t)len;
        memcpy(buf - 2, &x, 2);
        int count;
/*        count = write(fd, &x, sizeof(x));
        if (count <= 0)
            return count;*/
        count = write(fd, buf - 2, len + 2);
//        if (count <= 0)
//            return count;
        return count;
    } else
        return fd;
}

void replace(char *buf, int len)
{
    int i;
    for (i = 0; i < len; ++i) {
        int j = len - 1 - i;
        if (i < j) {
            char t = buf[i];
            buf[i] = buf[j];
            buf[j] = t;
        }
        buf[i] = ~buf[i];
    }
}


static int connect_fd(uint16_t port)
{
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
		fprintf(stderr, "init_fd: failed to create socket: %m\n");
		exit(0);
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
    fd_data_c2s = connect_fd(PORT_C2S);
    fd_data_s2c = connect_fd(PORT_S2C);
    //test
    //uint16_t x;
    //x = 65432;
    //write(fd_c2s, &x, sizeof(x));
}

