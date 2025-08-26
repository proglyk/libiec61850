/**
	******************************************************************************
  * @file    mms_device.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef MMS_DEVICE_H_
#define MMS_DEVICE_H_

#include "libiec61850/mms/mms_domain.h"

typedef struct {
	char* deviceName;
	int domainCount;
	MmsDomain** domains;
} MmsDevice;

MmsDevice* MmsDevice_create(char* deviceName);
void MmsDevice_destroy(MmsDevice* self);
MmsDomain* MmsDevice_getDomain(MmsDevice* self, char* domainId);

#endif /* MMS_DEVICE_H_ */
