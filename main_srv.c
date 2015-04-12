#include <stdio.h>
#include <stdlib.h>
#include "tcp.h"
#include "tun.h"

static void usage()
{
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
        //do some change
        socket_send(buf, len);
    }
}

char tbuf[4000];

int main(int argc, char **argv)
{
    tcpsrv_init();
    tun_init();
    
    pthread_t tid;puts("#1");
    pthread_create(&tid, NULL, process_tun, NULL);puts("#2");
    
    //main thread: handle TCP fd
    fd_set set;
    while (1) {
        FD_ZERO(&set);
        FD_SET(fd_c2s, &set);
        FD_SET(fd_s2c, &set);
        int maxsock = fd_c2s;
        if (fd_s2c > maxsock)
            maxsock = fd_s2c;
        /*
        int i;
        for (i = 0; i < fdlist_c2s_len; ++i) {
            int fd = fdlist_c2s[i];
            if (fd > maxsock)
                maxsock = fd;
            FD_SET(fd, &set);
        }*/
        if (fd_data_c2s > 0) {
            FD_SET(fd_data_c2s, &set);
            if (fd_data_c2s > maxsock)
                maxsock = fd_data_c2s;
        }    
        int ret = select(maxsock + 1, &set, NULL, NULL, NULL);
        if (ret < 0) {
            fprintf(stderr, "main: Error in select: %m\n");
            break;
        }
        if (FD_ISSET(fd_c2s, &set)) {
            printf("connect c2s\n");
            handle_c2s();
            continue;
        }
        if (FD_ISSET(fd_s2c, &set)) {
            printf("connect s2c\n");
            handle_s2c();
            continue;
        }
        printf("c2s socket read...");
        /*
        for (i = 0; i < fdlist_c2s_len; ++i) {
            int fd = fdlist_c2s[i];
            if (FD_ISSET(fd, &set)) {
                handle_socket(fd);
                break;
            }
        }*/
        if (fd_data_c2s > 0 && FD_ISSET(fd_data_c2s, &set)) {
            int count;
            if ((count = handle_socket(fd_data_c2s, tbuf)) <= 0) {
                fd_data_c2s = 0;
            }
            //do some change
            tun_send(tbuf, count);
        }

    }
}
