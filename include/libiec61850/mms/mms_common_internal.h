/**
	******************************************************************************
  * @file    mms_common_internal.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef __MMS_COMMON_INTERNAL_H__
#define __MMS_COMMON_INTERNAL_H__

/*******************************************************************************
	Defines
*******************************************************************************/


#include "libiec61850/mms/mms_value.h"
#include "iso9506/MmsPdu.h"


/*******************************************************************************
	Globally viewed routines prototypes
*******************************************************************************/


MmsValue*
mmsMsg_parseDataElement(Data_t* dataElement);

// void
// mmsMsg_createFloatData(MmsValue* value, int* size,  uint8_t** buf);

// Data_t*
// mmsMsg_createBasicDataElement(MmsValue* value);

// Data_t*
// mmsMsg_createDataElement(MmsValue* value);

// void
// mmsMsg_addResultToResultList(AccessResult_t* accessResult, MmsValue* value);

// AccessResult_t**
// mmsMsg_createAccessResultsList(MmsPdu_t* mmsPdu, int resultsCount);

// char*
// mmsMsg_createStringFromAsnIdentifier(Identifier_t identifier);

// void
// mmsMsg_deleteAccessResultList(AccessResult_t** accessResult, int variableCount);

#endif /*__MMS_COMMON_INTERNAL_H__*/