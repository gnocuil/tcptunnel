#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "tcp.h"
#include "tun.h"

static void usage()
{
    exit(1);
}

int main(int argc, char **argv)
{
	printf("REMOTE_IP_ADDR: %s\n", argv[argc - 1]);
	inet_pton(AF_INET, argv[argc - 1], &addr_remote);

	tcpcli_init();
    tun_init();
/*    
    //main thread: handle main TCP fd
	fd_set set;
    while (1) {
        FD_ZERO(&set);
        FD_SET(fd_c2s, &set);
        FD_SET(fd_s2c, &set);
	    int maxsock = fd_c2s;
	    if (fd_s2c > maxsock)
		    maxsock = fd_s2c;
        
        int ret = select(maxsock + 1, &set, NULL, NULL, NULL);
		if (ret < 0) {
			fprintf(stderr, "main: Error in select: %m\n");
			break;
		}
		if (FD_ISSET(fd_c2s, &set)) {
			printf("connect c2s\n");
			break;
		}
		if (FD_ISSET(fd_s2c, &set)) {
			printf("connect s2c\n");
			break;
		}
    }*/
}
