#ifndef SNMPSUPPORT_H
#define SNMPSUPPORT_H

#include <arpa/inet.h>

#if 0
typedef struct _tnet_network {
    struct in_addr ip_add;
    struct in_addr subnet;
    struct in_addr gateway;
    struct in_addr pri_dns;
    struct in_addr sec_dns;
    unsigned char dhcp_setting;
    unsigned char domain_name[48];
} tnet_network;
#endif

#if 0
typedef struct _wirelessinfo {
	char ssid[32 + 1];
	int type;
	char key[64 + 1];
	int channel;
	int control;
};
#endif

#endif

