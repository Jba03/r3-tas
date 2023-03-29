#ifndef stDsg_h
#define stDsg_h

#include "structure.h"

#define n_dsgvar_types              39

#define dsgvar_type_boolean          0
#define dsgvar_type_byte             1
#define dsgvar_type_ubyte            2
#define dsgvar_type_short            3
#define dsgvar_type_ushort           4
#define dsgvar_type_int              5
#define dsgvar_type_uint             6
#define dsgvar_type_float            7
#define dsgvar_type_vector           8
#define dsgvar_type_list             9
#define dsgvar_type_comport         10
#define dsgvar_type_action          11
#define dsgvar_type_capabilities    12
#define dsgvar_type_input           13
#define dsgvar_type_soundevent      14
#define dsgvar_type_light           15
#define dsgvar_type_game_material   16
#define dsgvar_type_visual_material 17
#define dsgvar_type_actor           18
#define dsgvar_type_waypoint        19
#define dsgvar_type_graph           20
#define dsgvar_type_text            21
#define dsgvar_type_superobject     22
#define dsgvar_type_so_links        23
#define dsgvar_type_actor_array     24
#define dsgvar_type_vector_array    25
#define dsgvar_type_float_array     26
#define dsgvar_type_int_array       27
#define dsgvar_type_waypoint_array  28
#define dsgvar_type_text_array      29
#define dsgvar_type_text_ref_array  30
#define dsgvar_type_graph_array     31
#define dsgvar_type_array9          32
#define dsgvar_type_sndevent_array  33
#define dsgvar_type_array11         34
#define dsgvar_type_way             35
#define dsgvar_type_action_array    36
#define dsgvar_type_superobj_array  37
#define dsgvar_type_object_list     38

static const char * const dsgvar_typenames[] = {
    "Boolean", "Byte", "UByte", "Short", "UShort", "Int",
    "UInt", "Float", "Vector", "List", "Comport", "Action", "Capabilities",
    "Input", "SoundEvent", "Light", "GameMaterial", "VisualMaterial", "Actor",
    "Waypoint", "Graph", "Text", "SuperObject", "SOLinks", "ActorArray",
    "VectorArray", "FloatArray", "IntArray", "WaypointArray", "TextArray",
    "TextRefArray", "GraphArray", "Array9", "SoundEventArray", "Array11",
    "Way", "ActionArray", "SuperObjectArray", "ObjectList", "Unknown",
};

struct stDsgVarInfo {
    readonly uint32 memOffset;
    readonly uint32 type;
    readonly int16 saveType;
    padding(2)
    readonly uint32 objectTreeInitialType;
} typedef tdstDsgVarInfo;

struct stDsgVar {
    readonly pointer memory;
    readonly pointer info;
    readonly uint32 memorySize;
    readonly uint8 infoLength;
    padding(3)
} typedef tdstDsgVar;

struct stDsgMem {
    readonly doublepointer dsgVars;
    readonly pointer initialBuffer;
    readonly pointer currentBuffer;
} typedef tdstDsgMem;

#endif /* stDsg_h */
