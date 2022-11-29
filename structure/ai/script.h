//
//  script.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#ifndef script_h
#define script_h

#include "stream.h"
#include "array.h"

#define SCRIPT

enum ScriptNodeType {
    NODE_TYPE_KEYWORD,
    NODE_TYPE_CONDITION,
    NODE_TYPE_OPERATOR,
    NODE_TYPE_FUNCTION,
    NODE_TYPE_PROCEDURE,
    NODE_TYPE_META_ACTION,
    NODE_TYPE_BEGIN_MACRO,
    NODE_TYPE_BEGIN_MACRO2,
    NODE_TYPE_END_MACRO,
    NODE_TYPE_FIELD,
    NODE_TYPE_DSGVARREF,
    NODE_TYPE_DSGVARREF2,
    NODE_TYPE_CONSTANT,
    NODE_TYPE_REAL,
    NODE_TYPE_BUTTON,
    NODE_TYPE_CONSTANT_VECTOR,
    NODE_TYPE_VECTOR,
    NODE_TYPE_MASK,
    NODE_TYPE_MODULEREF,
    NODE_TYPE_DSGVARID,
    NODE_TYPE_STRING,
    NODE_TYPE_LIPSSYNCHROREF,
    NODE_TYPE_FAMILYREF,
    NODE_TYPE_ACTORREF,
    NODE_TYPE_ACTIONREF,
    NODE_TYPE_SUPEROBJECTREF,
    NODE_TYPE_UNKNOWN,
    NODE_TYPE_WAYPOINTREF,
    NODE_TYPE_TEXTREF,
    NODE_TYPE_COMPORTREF,
    NODE_TYPE_MODULEREF2,
    NODE_TYPE_SOUNDEVENTREF,
    NODE_TYPE_OBJECTTABLEREF,
    NODE_TYPE_GAMEMATERIALREF,
    NODE_TYPE_VISUALMATERIAL,
    NODE_TYPE_PARTICLEGENERATOR,
    NODE_TYPE_MODELREF,
    NODE_TYPE_MODELREF2,
    NODE_TYPE_CUSTOMBITS,
    NODE_TYPE_CAPS,
    NODE_TYPE_UNKNOWN2,
    NODE_TYPE_SUBROUTINE,
    NODE_TYPE_NULL,
    NODE_TYPE_NULL2,
    NODE_TYPE_GRAPHREF,
};

struct ScriptNode {
    uint32_t param;
    uint8_t depth;
    
    enum ScriptNodeType type;
    struct Array* children;
    int line;
    int col;
};

struct Script { /* Tree */
    pointer ptr_scriptnodes_offset;
    struct Array* nodes;
    address offset;
};

struct ScriptInterface {
    
};

SCRIPT struct Script* script_read(const address addr);

SCRIPT const char* script_translate(struct Script* scpt);

#endif /* script_h */
