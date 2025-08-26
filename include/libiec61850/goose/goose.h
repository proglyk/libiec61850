/*
 *  goose.h
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

#ifndef GOOSE_H_
#define GOOSE_H_

#include "libiec61850/mms/mms_value.h"
#include <stdbool.h>
#include "libiec61850/utils/linked_list.h"
#include "libiec61850/utils/byte_buffer.h"

typedef struct sGooseControlBlock* GooseControlBlock;

struct sGooseControlBlock {

    char* goID;
    char* goCBRef;
    char* dataSetRef;

    uint16_t minTime;
    uint16_t maxTime;
    bool fixedOffs;

    uint32_t confRev;
    uint32_t stNum;
    uint32_t sqNum;
    uint32_t timeAllowedToLive;
    bool needsCommission;
    bool simulation;

    MmsValue* timestamp;
};

GooseControlBlock
GooseControlBlock_create();

void
GooseControlBlock_destroy(GooseControlBlock self);

int
GOOSE_createGoosePayload(GooseControlBlock gcb, LinkedList dataSetValues, ByteBuffer* buffer);

#endif /* GOOSE_H_ */
