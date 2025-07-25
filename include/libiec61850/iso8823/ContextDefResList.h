/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "ISO8823"
 * 	found in "iso8650\iso8650.asn"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_ContextDefResList_H_
#define	_ContextDefResList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <NativeInteger.h>
#include <OBJECT_IDENTIFIER.h>
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum result {
	result_acceptance	= 0,
	result_userrejection	= 1,
	result_providerrejection	= 2
} e_result;
typedef enum providerreason {
	providerreason_reasonnotspecified	= 0,
	providerreason_abstractsyntaxnotsupported	= 1,
	providerreason_proposedtransfersyntaxesnotsupported	= 2,
	providerreason_locallimitonDCSexceeded	= 3
} e_providerreason;


typedef struct {
	long	 result;
	OBJECT_IDENTIFIER_t	*transfersyntaxname	/* OPTIONAL */;
	long	*providerreason	/* OPTIONAL */;
		
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ContextDefResListSeq_t;

/* ContextDefResList */
typedef struct ContextDefResList {
	A_SEQUENCE_OF(ContextDefResListSeq_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ContextDefResList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ContextDefResList;

#ifdef __cplusplus
}
#endif

#endif	/* _ContextDefResList_H_ */
