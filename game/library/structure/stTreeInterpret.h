#ifndef stTreeInterpret_h
#define stTreeInterpret_h

#include "structure.h"
#include "game.h"

#define script_node_type_keyword            0
#define script_node_type_condition          1
#define script_node_type_operator           2
#define script_node_type_function           3
#define script_node_type_procedure          4
#define script_node_type_meta_action        5
#define script_node_type_begin_macro        6
#define script_node_type_begin_macro2       7
#define script_node_type_end_macro          8
#define script_node_type_field              9
#define script_node_type_dsgvarref          10
#define script_node_type_dsgvarref2         11
#define script_node_type_constant           12
#define script_node_type_real               13
#define script_node_type_button             14
#define script_node_type_constant_vector    15
#define script_node_type_vector             16
#define script_node_type_mask               17
#define script_node_type_moduleref          18
#define script_node_type_dsgvarid           19
#define script_node_type_string             20
#define script_node_type_lipssynchroref     21
#define script_node_type_familyref          22
#define script_node_type_actorref           23
#define script_node_type_actionref          24
#define script_node_type_superobjectref     25
#define script_node_type_solinksref         26 /* ? */
#define script_node_type_waypointref        27
#define script_node_type_textref            28
#define script_node_type_behaviorref        29
#define script_node_type_moduleref2         30
#define script_node_type_soundeventref      31
#define script_node_type_objecttableref     32
#define script_node_type_gamematerialref    33
#define script_node_type_visualmaterial     34
#define script_node_type_particlegenerator  35
#define script_node_type_modelref           36
#define script_node_type_modelref2          37
#define script_node_type_custombits         38
#define script_node_type_caps               39
#define script_node_type_graph              40 /* ? */
#define script_node_type_subroutine         41
#define script_node_type_null               42
#define script_node_type_cineref            43 /* ? */
#define script_node_type_graphref           44

typedef struct stNodeInterpret tdstNodeInterpret;
typedef struct stTreeInterpret tdstTreeInterpret;
typedef struct stMacroList tdstMacroList;
typedef struct stMacro tdstMacro;

struct stNodeInterpret
{
#if platform == GCN
    readonly uint32 param;
    padding(3)
    readonly uint8 type;
    padding(2)
    readonly uint8 depth;
    padding(1)
#endif
};

struct stTreeInterpret
{
    readonly pointer tree;
};

struct stMacro
{
    readonly char8 name[0x100];
    readonly pointer script_initial;
    readonly pointer script_current;
};

struct stMacroList
{
    readonly pointer macros;
    readonly uint8 n_macros;
    padding(3)
};

#if USE_FUNCTIONS

/** ai_function_temporal_real_combination: interpolation function based on engine frame time */
const float ai_function_temporal_real_combination(const float a, const float coefficient, const float b);

/** ai_function_get_time: get current timer count  */
const uint32 ai_function_get_time(void);

/** ai_function_get_time: get time elapsed since `v` */
const uint32 ai_function_get_elapsed_time(const uint32 v);

#endif

#endif /* stTreeInterpret_h */
