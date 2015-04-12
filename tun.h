#ifndef __TUN__
#define __TUN__

int tun_init();
int tun_send(char *packet, int len);
int handle_tun(char* buf, int buflen);

extern int tun_fd;

#endif
