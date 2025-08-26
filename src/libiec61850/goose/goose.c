#include "libiec61850/mms/mms_value.h"
#include "libiec61850/goose/goose.h"
#include "libiec61850/mms/mms_access_result.h"
#include "libiec61850/utils/ber_encoder.h"

static MmsValue* dataSetEntry = NULL;

GooseControlBlock
GooseControlBlock_create()
{
    GooseControlBlock self = calloc(1, sizeof(struct sGooseControlBlock));
		
		return self;
}

void
GooseControlBlock_destroy(GooseControlBlock self)
{
    if (self->timestamp != NULL)
        MmsValue_delete(self->timestamp);

    if (self->dataSetRef != NULL)
        free(self->dataSetRef);

    if (self->goID != NULL)
        free(self->goID);

    if (self->goCBRef != NULL)
        free(self->goCBRef);

    free(self);
}

int
GOOSE_createGoosePayload(GooseControlBlock gcb, LinkedList dataSetValues, ByteBuffer* byteBuffer) {
	
	
	
  /* Step 1 - calculate length fields */
  uint32_t goosePduLength = 0;

  goosePduLength += BerEncoder_determineEncodedStringSize(gcb->goCBRef);

  goosePduLength += BerEncoder_determineEncodedStringSize(gcb->dataSetRef);

  if (gcb->goID != NULL)
  	goosePduLength += BerEncoder_determineEncodedStringSize(gcb->goID);

  uint32_t timeAllowedToLive = gcb->timeAllowedToLive;

  goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(timeAllowedToLive);

  goosePduLength += 2 + 8; /* for T (UTCTIME) */

  goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(gcb->sqNum);

  goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(gcb->stNum);

  goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(gcb->confRev);

  goosePduLength += 6; /* for ndsCom and simulation */

  uint32_t numberOfDataSetEntries = LinkedList_size(dataSetValues);

  goosePduLength += 2 + BerEncoder_UInt32determineEncodedSize(numberOfDataSetEntries);

    uint32_t dataSetSize = 0;

    LinkedList element = LinkedList_getNext(dataSetValues);

    while (element != NULL) {
        dataSetEntry = (MmsValue*) element->data;

        dataSetSize += mmsServer_encodeAccessResult(dataSetEntry, NULL, 0, false);

        element = LinkedList_getNext(element);
    }

	uint32_t allDataSize = dataSetSize + BerEncoder_determineLengthSize(dataSetSize) + 1;

	goosePduLength += allDataSize;

	/* Step 2 - encode to buffer */

	int bufPos = 0;
	uint8_t* buffer = ByteBuffer_getBuffer(byteBuffer);

	/* Encode GOOSE PDU */
	bufPos = BerEncoder_encodeTL(0x61, goosePduLength, buffer, bufPos);

	/* Encode gocbRef */
	bufPos = BerEncoder_encodeStringWithTag(0x80, gcb->goCBRef, buffer, bufPos);

	/* Encode timeAllowedToLive */
	bufPos = BerEncoder_encodeUInt32WithTL(0x81, timeAllowedToLive, buffer, bufPos);

	/* Encode datSet reference */
	bufPos = BerEncoder_encodeStringWithTag(0x82, gcb->dataSetRef, buffer, bufPos);

	/* Encode goID */
	if (gcb->goID != NULL)
		bufPos = BerEncoder_encodeStringWithTag(0x83, gcb->goID, buffer, bufPos);

	/* Encode t */
	bufPos = BerEncoder_encodeOctetString(0x84, gcb->timestamp->value.utcTime, 8, buffer, bufPos);

	/* Encode stNum */
	bufPos = BerEncoder_encodeUInt32WithTL(0x85, gcb->stNum, buffer, bufPos);

	/* Encode sqNum */
	bufPos = BerEncoder_encodeUInt32WithTL(0x86, gcb->sqNum, buffer, bufPos);

	/* Encode simulation */
	bufPos = BerEncoder_encodeBoolean(0x87, gcb->simulation, buffer, bufPos);

	/* Encode confRef */
	bufPos = BerEncoder_encodeUInt32WithTL(0x88, gcb->confRev, buffer, bufPos);

	/* Encode ndsCom */
	bufPos = BerEncoder_encodeBoolean(0x89, gcb->needsCommission, buffer, bufPos);

	/* Encode numDatSetEntries */
	bufPos = BerEncoder_encodeUInt32WithTL(0x8a, numberOfDataSetEntries, buffer, bufPos);

	/* Encode all data */
	bufPos = BerEncoder_encodeTL(0xab, dataSetSize, buffer, bufPos);

	/* Encode data set entries */
	element = LinkedList_getNext(dataSetValues);
		
	dataSetEntry = NULL;

	while (element != NULL) {
		dataSetEntry = (MmsValue*) element->data;

		bufPos = mmsServer_encodeAccessResult(dataSetEntry, buffer, bufPos, true);

		element = LinkedList_getNext(element);
	}
	
	return bufPos;
}

