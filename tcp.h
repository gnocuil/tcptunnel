#ifndef __TCP__
#define __TCP__

void tcpsrv_init();

void tcpcli_init();

extern int fd_c2s;
extern int fd_s2c;

extern struct in_addr addr_remote;

#endif
