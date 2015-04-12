#pragma once

#define TUNNEL_NAME "4over6"

int tun_create(char *dev);
int tun_send(char *packet, int len);
int handle_tun();

extern struct in_addr addr_local, addr_remote;
extern unsigned short port_local, port_remote;

