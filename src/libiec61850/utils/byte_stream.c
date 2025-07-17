#include "stack_config.h"
#include "libiec61850/utils/byte_stream.h"
//#include "server\hal.h"
//#include "server\stack_config.h."
//#include "lwip/opt.h"
#include "time_hal.h"
#include "lwip/sockets.h"

static unsigned char recv_buffer[64];

ByteStream
ByteStream_create(s32_t socket, ByteBuffer* writeBuffer)
{
	ByteStream self = calloc(1, sizeof(struct sByteStream));

	self->socket = socket;
	self->writeBuffer = writeBuffer;

	return self;
}

void
ByteStream_destroy(ByteStream self)
{
	free(self);
}

int
ByteStream_writeUint8(ByteStream self, uint8_t byte)
{
	if (ByteBuffer_appendByte(self->writeBuffer, byte))
		return 1;
	else
		return -1;
}

int
ByteStream_readUint8(ByteStream self, uint8_t* byte)
{
	int bytes_read;
	uint64_t start = Hal_getTimeInMs();
	
//	bytes_read = Socket_read(self->socket, recv_buffer, 64);
	bytes_read = lwip_read(self->socket, byte, 1);

/*	do {
		bytes_read = Socket_read(self->socket, recv_buffer, 64);
		//bytes_read = -1;
	} while ((bytes_read == 0) && ((Hal_getTimeInMs() - start) < CONFIG_TCP_READ_TIMEOUT_MS));*/

/*	if (COTP_DEBUG) {
		if (bytes_read == 1) {
			//printf("errno: %i\r\n", errno);
			LWIP_DEBUGF(COTP_DEBUG, ("bytes_read=: %c\r\n", (char)*byte));
		}
	}*/

//	if (bytes_read <= 0) {
	if (bytes_read != 1) {
		return -1;
	}

	return 1;
}

int
ByteStream_read(ByteStream self, uint8_t* byte, int lenght)
{
	int bytes_read;

	bytes_read = lwip_read(self->socket, byte, lenght);

	if (bytes_read != lenght)
		return -1;

	return 1;
}

int
ByteStream_readUint16(ByteStream self, uint16_t* value)
{
	uint8_t byte[2];
	int bytes_read;

	uint64_t start = Hal_getTimeInMs();

	bytes_read = lwip_read(self->socket, byte, 2);

/*	do {
		bytes_read = Socket_read(self->socket, byte, 2);
	} while ((bytes_read == 0)
			&& ((Hal_getTimeInMs() - start) < CONFIG_TCP_READ_TIMEOUT_MS));*/

	if (bytes_read != 2) {
		return -1;
	}

	*value = (byte[0] * 0x100) + byte[1];
	return 2;
}

int
ByteStream_readOctets(ByteStream self, uint8_t* buffer, int size)
{
	int readBytes = 0;
	int remainingSize = size;

	uint64_t start = Hal_getTimeInMs();

	do {
		int chunkSize = lwip_read(self->socket, buffer + readBytes, remainingSize);
		if (chunkSize < 0)
			return -1;
		else
		{
			readBytes += chunkSize;
			remainingSize = size - readBytes;
		}
	} while ((readBytes < size)
			&& ((Hal_getTimeInMs() - start) < CONFIG_TCP_READ_TIMEOUT_MS));

	return readBytes;
}

int
ByteStream_sendBuffer(ByteStream self)
{
	int writeBufferPosition = ByteBuffer_getSize(self->writeBuffer);

	if (lwip_write(self->socket, ByteBuffer_getBuffer(self->writeBuffer), writeBufferPosition)
			== writeBufferPosition)
	{
		ByteBuffer_setSize(self->writeBuffer, 0);
		return 1;
	}
	else
		return -1;
}

void
ByteStream_setWriteBuffer(ByteStream self, ByteBuffer* writeBuffer)
{
    self->writeBuffer = writeBuffer;
}
