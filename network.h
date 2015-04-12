#pragma once

#include "encap.h"

extern unsigned mtu;

int set_mtu(char *interface_name, unsigned mtu);
int interface_up(char *interface_name);
//int set_ipaddr(char *interface_name, char *ip);

extern Encap *encap;
