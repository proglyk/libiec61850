#ifndef __WRITERESPONSELIST_H__
#define __WRITERESPONSELIST_H__

#include <asn_application.h>
#include "DataAccessError.h"
#include <NULL.h>
#include <constr_CHOICE.h>

/* Dependencies */
typedef enum {
	WriteResponseList_PR_NOTHING,	/* No components present */
	WriteResponseList_PR_failure,
	WriteResponseList_PR_success
} WriteResponseList_PR;

typedef struct {
	WriteResponseList_PR present;
	union {
		DataAccessError_t	 failure;
		NULL_t	 success;
	} choice;
		
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} WriteResponseList_t;


#endif /*__WRITERESPONSELIST_H__*/
