/**
	******************************************************************************
  * @file    mms_type_spec.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef MMS_TYPE_SPEC_H_
#define MMS_TYPE_SPEC_H_

#include "libiec61850/mms/mms_types.h"
#include <stdint.h>


/**
 * Delete MmsTypeSpecification object (recursive).
 */
void MmsTypeSpecification_delete(MmsTypeSpecification*);
MmsValue *MmsTypeSpecification_getChildValue(MmsTypeSpecification*, MmsValue*, char*);

#endif /* MMS_TYPE_SPEC_H_ */
