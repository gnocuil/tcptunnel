#pragma once

int socket_init();
int socket_init_tun();
int handle_socket();
int socket_send(char *buf, int len);
int socket_send4(char *buf, int len);
