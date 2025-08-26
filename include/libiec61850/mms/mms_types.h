/**
	******************************************************************************
  * @file    mms_types.h
  * @author  Michael Zillgith
  * @brief   This file is part of libIEC61850.
  *****************************************************************************/

#ifndef MMS_TYPES_H_
#define MMS_TYPES_H_

#include <stdint.h>
#include "libiec61850/utils/ber_integer.h"
//#include "ied/mms/mms_domain.h"


#define DEFAULT_MAX_SERV_OUTSTANDING_CALLING 5
#define DEFAULT_MAX_SERV_OUTSTANDING_CALLED 5
#define DEFAULT_DATA_STRUCTURE_NESTING_LEVEL 10
#define DEFAULT_MAX_PDU_SIZE 65000


typedef enum {
	MMS_VALUE_OK,
	MMS_VALUE_ACCESS_DENIED,
	MMS_VALUE_VALUE_INVALID
} MmsValueIndication;

typedef enum {
	MMS_ERROR, MMS_INITIATE, MMS_CONFIRMED_REQUEST, MMS_OK, MMS_CONCLUDE
} MmsIndication;

//CHANGE VER_0_2_0
/*typedef enum {
	MMS_NAMED_VARIABLE,
	MMS_NAMED_VARIABLE_LIST
} MmsObjectClass;*/

typedef enum {
	MMS_ARRAY,
	MMS_STRUCTURE,
	MMS_BOOLEAN,
	MMS_BIT_STRING,
	MMS_INTEGER,
	MMS_UNSIGNED,
	MMS_FLOAT,
	MMS_OCTET_STRING,
	MMS_VISIBLE_STRING,
	MMS_GENERALIZED_TIME,
	MMS_BINARY_TIME,
	MMS_BCD,
	MMS_OBJ_ID,
	MMS_STRING,
	MMS_UTC_TIME,
	MMS_DATA_ACCESS_ERROR	//CHANGE _V_0_3_1_
} MmsType;

/**
 * MmsValue - complex value type for MMS Client API
 */
typedef struct sMmsValue MmsValue;

struct sMmsValue {
	MmsType type;
	int deleteValue;
	union uMmsValue {
		struct {	//CHANGE _V_0_3_1_
			uint32_t code;
    } dataAccessError;
		struct {
			int size;
			MmsValue** components;
		} array;
		struct {
			int componentCount;
			MmsValue** components;
		} structure;
		int boolean;
		Asn1PrimitiveValue* integer;
		Asn1PrimitiveValue* unsignedInteger;
		struct {
			uint8_t exponentWidth;
			uint8_t formatWidth;
			uint8_t* buf;
		} floatingPoint;
		struct {
			uint16_t size;
			uint16_t maxSize;
			uint8_t* buf;
		} octetString;
		struct {
			int size;     /* Number of bits */
			uint8_t* buf;
		} bitString;
		char* mmsString;
		char* visibleString;
		uint8_t utcTime[8];
		struct {
			uint8_t size;
			uint8_t buf[6];
		} binaryTime;
	} value;
};

/**
 * Type definition for MMS Named Variables
 */
typedef struct sMmsTypeSpecification MmsTypeSpecification;

struct sMmsTypeSpecification {
	MmsType type;
	char* name;
	union uMmsTypeSpecification {
		struct sMmsArray {
			int elementCount;		/* number of array elements */
			MmsTypeSpecification* elementTypeSpec;
		} array;
		struct sMmsStructure {
			int elementCount;
			MmsTypeSpecification** elements;
		} structure;
		int boolean; 				/* dummy - not required */
		int integer; 				/* size of integer in bits */
		int unsignedInteger; 		/* size of integer in bits */
		struct sMmsFloat {
			uint8_t exponentWidth;
			uint8_t formatWidth;
		} floatingpoint;
		int bitString; 				/* Number of bits in bitstring */
		int octetString; 			/* Number of octets in octet string */
		int visibleString;			/* Maximum size of string */
		int mmsString;
		int utctime;				/* dummy - not required */
		int binaryTime;			    /* size: either 4 or 6 */
	} typeSpec;
};


#endif /* MMS_TYPES_H_ */
