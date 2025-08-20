#include "ied_server.h"

/**	----------------------------------------------------------------------------
	* @brief Constructor */
IedServer
	IedServer_create(IedModel* iedModel) {
/*----------------------------------------------------------------------------*/
	IedServer self = calloc(1, sizeof(struct sIedServer));

/* 	self->model = iedModel;

	self->mmsMapping = MmsMapping_create(iedModel);

	self->mmsDevice = MmsMapping_getMmsDeviceModel(self->mmsMapping);

	//self->isoServer = IsoServer_create();

	self->mmsServer = MmsServer_create(NULL, self->mmsDevice);
	
	MmsMapping_setMmsServer(self->mmsMapping, self->mmsServer);

	MmsMapping_installHandlers(self->mmsMapping);

	createMmsServerCache(self);

	iedModel->initializer();

	installDefaultValuesInCache(self);

	updateDataSetsWithCachedValues(self); */

	return self;
}