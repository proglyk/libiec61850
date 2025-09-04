/*
 *  goose_publisher.c
 *
 *  Copyright 2013 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "stack_config.h"
#include "libiec61850/goose/goose.h"
#include "libiec61850/goose/goose_publisher.h"
#include "goose_raw.h"
//#include "stm32f4xx_hal.h"
//#include "heap_user.h"

#define GOOSE_MAX_MESSAGE_SIZE 512

static uint8_t buffer[GOOSE_MAX_MESSAGE_SIZE];

struct sGoosePublisher {
    //uint8_t* buffer;
    uint16_t appId;
    EthernetSocketPtr ethernetSocket;
    //int lengthField;
    //int payloadStart;
};

typedef struct sCommParameters {
    uint8_t vlanPriority;
    uint16_t vlanId;
    uint16_t appId;
    uint8_t dstAddress[6];
} CommParameters;

GoosePublisher*
GoosePublisher_create(CommParameters* parameters, char* interfaceID)
{
    GoosePublisher* self = calloc(1, sizeof(GoosePublisher));

    uint8_t srcAddr[6];
		
		//GOOSE_NO_ETH
    //if (interfaceID != NULL)
    //    Ethernet_getInterfaceMACAddress(interfaceID, srcAddr);
    //else
     //   Ethernet_getInterfaceMACAddress(CONFIG_ETHERNET_INTERFACE_ID, srcAddr);

    uint8_t defaultDstAddr[] = CONFIG_GOOSE_DEFAULT_DST_ADDRESS;

    uint8_t* dstAddr;
    uint8_t priority;
    uint16_t vlanId;
    uint16_t appId;
		int bufPos = 0;

    if (parameters == NULL) {
        dstAddr = defaultDstAddr;
        priority = CONFIG_GOOSE_DEFAULT_PRIORITY;
        vlanId = CONFIG_GOOSE_DEFAULT_VLAN_ID;
        appId = CONFIG_GOOSE_DEFAULT_APPID;
    }
    else {
        dstAddr = parameters->dstAddress;
        priority = parameters->vlanPriority;
        vlanId = parameters->vlanId;
        appId = parameters->appId;
    }

		//GOOSE_NO_ETH
    // TODO self->ethernetSocket = Ethernet_createSocket("eth0", dstAddr);
		
		memset(buffer, 0, GOOSE_MAX_MESSAGE_SIZE*sizeof(uint8_t));

/*
    memcpy(self->buffer, dstAddr, 6);
    memcpy(self->buffer + 6, srcAddr, 6);

    int bufPos = 12;

    // Priority tag - IEEE 802.1Q 
    self->buffer[bufPos++] = 0x81;
    self->buffer[bufPos++] = 0x00;

    uint8_t tci1 = priority << 5;
    tci1 += vlanId / 256;

    uint8_t tci2 = vlanId % 256;

    self->buffer[bufPos++] = tci1; // Priority + VLAN-ID
    self->buffer[bufPos++] = tci2; // VLAN-ID

    // EtherType GOOSE
    self->buffer[bufPos++] = 0x88;
    self->buffer[bufPos++] = 0xB8;

    // APPID
    self->buffer[bufPos++] = appId / 256;
    self->buffer[bufPos++] = appId % 256;

    self->lengthField = bufPos;

    // Length
    self->buffer[bufPos++] = 0x00;
    self->buffer[bufPos++] = 0x08;

    // Reserved1
    self->buffer[bufPos++] = 0x00;
    self->buffer[bufPos++] = 0x00;

    // Reserved2
    self->buffer[bufPos++] = 0x00;
    self->buffer[bufPos++] = 0x00;*/

    //self->payloadStart = bufPos;

    return self;
}

void
GoosePublisher_destroy(GoosePublisher* self)
{
	//GOOSE_NO_ETH
	//Ethernet_destroySocket(self->ethernetSocket);
	//free(self->buffer);
	free(self);
}

int
GoosePublisher_publish(GoosePublisher* self, GooseControlBlock gcb, LinkedList dataSet)
{
    ByteBuffer payloadBuffer;
	
    ByteBuffer_wrap(&payloadBuffer, buffer, 0, GOOSE_MAX_MESSAGE_SIZE);

    int payloadLength = GOOSE_createGoosePayload(gcb, dataSet, &payloadBuffer);

    //int lengthIndex = self->lengthField;

    //self->buffer[lengthIndex] = payloadLength / 256;
    //self->buffer[lengthIndex + 1] = payloadLength & 0xff;

		//GOOSE_NO_ETH
    //Ethernet_sendPacket(self->ethernetSocket, self->buffer, self->payloadStart + payloadLength);
		Ethernet_send(self->ethernetSocket, (uint8_t*)payloadBuffer.buffer, payloadLength);

    int res = 0;

    return res;
}
