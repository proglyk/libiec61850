#include "libiec61850/mms/mms_device.h"
#include <stdlib.h>
#include <string.h>
//#include "heap_user.h"



MmsDevice*
MmsDevice_create(char* deviceName)
{
	MmsDevice* self = calloc(1, sizeof(MmsDevice));
	self->deviceName = deviceName;

	return self;
}

void
MmsDevice_destroy(MmsDevice* self) {

	int i;
	for (i = 0; i < self->domainCount; i++) {
		MmsDomain_destroy(self->domains[i]);
	}

	free(self->domains);
	free(self);
}



MmsDomain*
MmsDevice_getDomain(MmsDevice* self, char* domainId)
{
	int i;

	for (i = 0; i < self->domainCount; i++) {
		if (strcmp(self->domains[i]->domainName, domainId) == 0) {
			return self->domains[i];
		}

	}

	return NULL;
}


