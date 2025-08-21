#ifndef _listOfAccessResult_H_
#define _listOfAccessResult_H_

#include <asn_application.h>

#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>


typedef struct listOfAccessResult {
	A_SEQUENCE_OF(struct AccessResult) list;

	asn_struct_ctx_t _asn_ctx;
} listOfAccessResult_t;


#endif /*_listOfAccessResult_H_*/
