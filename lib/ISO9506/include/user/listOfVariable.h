#ifndef _listOfVariable_H_
#define _listOfVariable_H_

#include <asn_application.h>

#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>


typedef struct listOfVariable {
		A_SEQUENCE_OF(struct ListOfVariableSeq) list;

		asn_struct_ctx_t _asn_ctx;
	} listOfVariable_t;


#endif /*_listOfVariable_H_*/
