#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "tcp.h"
#include "tun.h"

static void usage()
{
    fprintf(stderr, "Usage: tcpcli <REMOTE_IPv4_ADDR>\n");
    exit(1);
}

static void* process_tun(void* arg)
{
    printf("process_tun!!!\n");
    char buf[4000];
    while (1) {
        int len = handle_tun(buf, 4000);
        if (len <= 0) {
            printf("Error handling TUN, QUIT!!!\n");
            exit(0);
        }
//        replace(buf, len);
        int count = socket_send(fd_data_c2s, buf, len);
        if (count <= 0) {
            printf("Error sending socket , QUIT!!!\n");
            exit(0);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
        usage();
	printf("REMOTE_IP_ADDR: %s\n", argv[argc - 1]);
	inet_pton(AF_INET, argv[argc - 1], &addr_remote);

	tcpcli_init();
    tun_init();
    
    pthread_t tid;puts("#1");
    pthread_create(&tid, NULL, process_tun, NULL);puts("#2");
    
   
    //main thread: handle TCP fd
    char buf[4000];
    while (1) {
        int count;
        if ((count = handle_socket(fd_data_s2c, buf)) <= 0) {
           fprintf(stderr, "Error reading socket! QUIT...\n");
           exit(0);
        } else {
            replace(buf, count);
            tun_send(buf, count);
        }
    }
}
