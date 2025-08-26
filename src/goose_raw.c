#include "ethernetif/goose_raw.h"
#include "lwip/tcpip.h"
#include <string.h>
#include "lwip/sockets.h"
#include "stdbool.h"
#include "lwip/netif.h"
#include "stm32f4xx_hal.h"
#include "ethernetif/eth.h"

//extern struct netif gnetif;
static EthernetSocketPtr socket;
int var;
char data[4] = {'a','b','c','d',};
extern Ethernet xEthernet;


void
	Ethernet_test(void) {
		
	LWIP_DEBUGF(RAW_DEBUG, ("raw: Ethernet_createSocket\r\n"));
	socket = Ethernet_createSocket(NULL, NULL);
	LWIP_DEBUGF(RAW_DEBUG, ("raw: Ethernet_send\r\n"));
	var = Ethernet_send(socket, (uint8_t*)data, 4);
	
	__NOP();
}

EthernetSocketPtr
	Ethernet_createSocket(char* interfaceId, uint8_t* destAddress) {
	
	EthernetSocketPtr	raw;
	struct sockaddr_in local;
		
	raw = (EthernetSocketPtr)calloc(1, sizeof(EthernetSocket));
		
	if ((raw->sock = lwip_socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
		goto destroy;
		
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = PP_HTONS(102);
	local.sin_addr.s_addr = xEthernet.xNetif.ip_addr.addr;
		
	if (lwip_bind(raw->sock, (struct sockaddr *)&local, sizeof(local)) < 0)
		goto destroy;
		
	memset(&(raw->remote), 0, sizeof(raw->remote));
	raw->remote.sin_family = AF_INET;
	raw->remote.sin_port = PP_HTONS(102);
	raw->remote.sin_addr.s_addr = 0;
	//inet_addr_from_ipaddr(&remote.sin_addr, &sntp_server_address);
		
	return raw;
	
	destroy:
	free(raw);
	return NULL;	
}

int
	Ethernet_send(EthernetSocketPtr eth, uint8_t* buf, int size) {
		
	return lwip_sendto(eth->sock, (const void *)buf, size, 0, 
		(const struct sockaddr *)&(eth->remote), sizeof(eth->remote));
}