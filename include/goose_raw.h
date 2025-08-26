#ifndef __GOOSE_RAW_H__
#define __GOOSE_RAW_H__

#include "stdint.h"
#include "lwip/sockets.h"
#include "lwip/prot/ethernet.h"

// Ќнадо временно добавить сюда натсройку dhcp
//#define GOOSE_PUBLISHER_ADDR (char*)"85.21.78.91"

typedef struct {
	int sock;
	struct sockaddr_in remote;
} EthernetSocket;

typedef struct {
	u8_t hwaddr_dst[ETH_HWADDR_LEN];
	u8_t hwaddr_src[ETH_HWADDR_LEN];
} goose_msg_t;


typedef EthernetSocket * EthernetSocketPtr;

EthernetSocketPtr
	Ethernet_createSocket(char* interfaceId, uint8_t* destAddress);
	
int
	Ethernet_send(EthernetSocketPtr eth, uint8_t* buf, int size);

void
	Ethernet_test(void);

#endif /*__GOOSE_RAW_H__*/