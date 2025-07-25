/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "ISO8823"
 * 	found in "iso8650\iso8650.asn"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_CPType_H_
#define	_CPType_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ModeSelector.h"
#include "ProtocolVersion.h"
#include <OCTET_STRING.h>
#include "Requirements.h"
#include "UserSessionRequirements.h"
#include <constr_SEQUENCE.h>
#include <constr_SET.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */

/*
 * Method of determining the components presence
 */
typedef enum CPType_PR {
	CPType_PR_modeSelector,	/* Member modeSelector is present */
	CPType_PR_normalModeParameters,	/* Member normalModeParameters is present */
} CPType_PR;

/* Forward declarations */
struct ContextList;
struct DefaultContextName;
struct Userdata;

/* CPType */
typedef struct CPType {
	ModeSelector_t	 modeSelector;
	struct {
		ProtocolVersion_t	*protocolVersion	/* DEFAULT {version1} */;
		OCTET_STRING_t	*callingSelector	/* OPTIONAL */;
		OCTET_STRING_t	*calledSelector	/* OPTIONAL */;
		struct ContextList	*contextDefinitionList	/* OPTIONAL */;
		struct DefaultContextName	*defaultContextName	/* OPTIONAL */;
		Requirements_t	*requirements	/* OPTIONAL */;
		UserSessionRequirements_t	*userSessionRequirements	/* OPTIONAL */;
		struct Userdata	*userData	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} normalModeParameters;
	
	/* Presence bitmask: ASN_SET_ISPRESENT(pCPType, CPType_PR_x) */
	unsigned int _presence_map
		[((2+(8*sizeof(unsigned int))-1)/(8*sizeof(unsigned int)))];
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CPType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CPType;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ContextList.h"
#include "DefaultContextName.h"
#include "Userdata.h"

#endif	/* _CPType_H_ */
