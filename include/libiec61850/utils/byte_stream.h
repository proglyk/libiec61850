#ifndef BYTE_STREAM_H_
#define BYTE_STREAM_H_

//#include "platform_types.h"
#include "userint.h"
//#include "socket2.h"
#include "libiec61850/utils/byte_buffer.h"

typedef struct sByteStream {
	s32_t socket;
	ByteBuffer* writeBuffer;
}* ByteStream;

ByteStream ByteStream_create(s32_t socket, ByteBuffer* writeBuffer);
int ByteStream_writeUint8(ByteStream self, uint8_t byte);
int ByteStream_readUint16(ByteStream self, uint16_t* value);
int ByteStream_readUint8(ByteStream self, uint8_t* byte);
int ByteStream_sendBuffer(ByteStream self);
int ByteStream_read(ByteStream self, uint8_t* byte, int lenght);
void ByteStream_destroy(ByteStream self);
int ByteStream_readOctets(ByteStream self, uint8_t* targetBuffer, int size);
void
ByteStream_setWriteBuffer(ByteStream self, ByteBuffer* writeBuffer);

/*
#include "platform_types.h"
#include "socket.h"
#include "byte_buffer.h"

int
ByteStream_skipBytes(ByteStream self, int number);


*/
#endif /* BYTE_STREAM_H_ */
