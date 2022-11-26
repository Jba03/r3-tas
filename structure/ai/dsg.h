//
//  dsg.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#ifndef dsg_h
#define dsg_h

#include "memory.h"

#define DSG

#define N_DSGVAR_TYPES 40

#define DSGVAR_TYPE_NONE             0
#define DSGVAR_TYPE_BOOLEAN          1
#define DSGVAR_TYPE_BYTE             2
#define DSGVAR_TYPE_UBYTE            3
#define DSGVAR_TYPE_SHORT            4
#define DSGVAR_TYPE_USHORT           5
#define DSGVAR_TYPE_INT              6
#define DSGVAR_TYPE_UINT             7
#define DSGVAR_TYPE_FLOAT            8
#define DSGVAR_TYPE_VECTOR           9
#define DSGVAR_TYPE_LIST            10
#define DSGVAR_TYPE_COMPORT         11
#define DSGVAR_TYPE_ACTION          12
#define DSGVAR_TYPE_CAPABILITIES    13
#define DSGVAR_TYPE_INPUT           14
#define DSGVAR_TYPE_SOUNDEVENT      15
#define DSGVAR_TYPE_LIGHT           16
#define DSGVAR_TYPE_GAME_MATERIAL   17
#define DSGVAR_TYPE_VISUAL_MATERIAL 18
#define DSGVAR_TYPE_ACTOR           19
#define DSGVAR_TYPE_WAYPOINT        20
#define DSGVAR_TYPE_GRAPH           21
#define DSGVAR_TYPE_TEXT            22
#define DSGVAR_TYPE_SUPEROBJECT     23
#define DSGVAR_TYPE_SO_LINKS        24
#define DSGVAR_TYPE_ACTOR_ARRAY     25
#define DSGVAR_TYPE_VECTOR_ARRAY    26
#define DSGVAR_TYPE_FLOAT_ARRAY     27
#define DSGVAR_TYPE_INT_ARRAY       28
#define DSGVAR_TYPE_WAYPOINT_ARRAY  29
#define DSGVAR_TYPE_TEXT_ARRAY      30
#define DSGVAR_TYPE_TEXT_REF_ARRAY  31
#define DSGVAR_TYPE_GRAPH_ARRAY     32
#define DSGVAR_TYPE_ARRAY9          33
#define DSGVAR_TYPE_SNDEVENT_ARRAY  34
#define DSGVAR_TYPE_ARRAY11         35
#define DSGVAR_TYPE_WAY             36
#define DSGVAR_TYPE_ACTION_ARRAY    37
#define DSGVAR_TYPE_SUPEROBJ_ARRAY  38
#define DSGVAR_TYPE_OBJECT_LIST     39

static const char * const dsgvar_typenames[] = {
    "None", "Boolean", "Byte", "UByte", "Short", "UShort", "Int",
    "UInt", "Float", "Vector", "List", "Comport", "Action", "Capabilities",
    "Input", "SoundEvent", "Light", "GameMaterial", "VisualMaterial", "Actor",
    "Waypoint", "Graph", "Text", "SuperObject", "SOLinks", "ActorArray",
    "VectorArray", "FloatArray", "IntArray", "WaypointArray", "TextArray",
    "TextRefArray", "GraphArray", "Array9", "SoundEventArray", "Array11",
    "Way", "ActionArray", "SuperObjectArray", "ObjectList",
    "Unknown",
};

struct DSGVariableInfo {
    pointer i_mem_offset; /* not to be used outside dsg.c */
    pointer data_offset; /* use this instead */
    
    uint32_t type_id;
    uint32_t save_type;
    uint32_t init_type;
    
    const char* type_name;
    
    address offset;
};

struct DSGVariable {
    pointer ptr_mem_offset;
    pointer ptr_info;
    
    uint32_t memory_length;
    uint8_t info_length;
    
    struct DSGVariableInfo* entries;
    
    address offset;
};

struct DSGMemory {
    pointer ptr_dsgvars;
    pointer ptr_memory_current;
    pointer ptr_memory_initial;
    
    struct DSGVariable* var;
    struct DSGVariableInfo* initial;
    struct DSGVariableInfo* current;
    uint8_t n_variables;
    
    address offset;
};

DSG struct DSGMemory* dsgmemory_read(const address addr);

#endif /* dsg_h */
