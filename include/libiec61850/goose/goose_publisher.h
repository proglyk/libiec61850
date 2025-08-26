#ifndef GOOSE_PUBLISHER_H_
#define GOOSE_PUBLISHER_H_

#include "libiec61850/goose/goose.h"

typedef struct sGoosePublisher GoosePublisher;

GoosePublisher*
GoosePublisher_create();

void
GoosePublisher_destroy(GoosePublisher* self);

int
GoosePublisher_publish(GoosePublisher* self, GooseControlBlock gcb, LinkedList dataSet);

#endif /* GOOSE_PUBLISHER_H_ */
