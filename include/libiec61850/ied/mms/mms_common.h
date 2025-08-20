#ifndef _MMS_COMMON_H_
#define _MMS_COMMON_H_

typedef struct sMmsDomain MmsDomain;

typedef struct sMmsAccessSpecifier {
    MmsDomain* domain;
    char* variableName;
    int arrayIndex; /* -1 --> no index present / ignore index */
    char* componentName;
} MmsAccessSpecifier;

typedef struct sMmsAccessSpecifier* MmsNamedVariableListEntry;

typedef struct sMmsNamedVariableList* MmsNamedVariableList;

typedef struct sMmsServerConnection MmsServerConnection;
// temp
struct sMmsServerConnection {
	void* null;
};

#endif /* _MMS_COMMON_H_ */
