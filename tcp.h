#ifndef __TCP__
#define __TCP__

void tcpsrv_init();
void handle_c2s();
void handle_s2c();

int handle_socket(int fd, char *buf);

int socket_send(int fd, char* buf, int len);

void replace(char *buf, int len);//change a packet

void tcpcli_init();

extern int fd_c2s;
extern int fd_s2c;

#define LIST_MAX 1000

//extern int fdlist_s2c[LIST_MAX];
//extern int fdlist_c2s[LIST_MAX];
//extern int fdlist_s2c_len;
//extern int fdlist_c2s_len;
extern int fd_data_c2s;
extern int fd_data_s2c;


extern struct in_addr addr_remote;

#endif
