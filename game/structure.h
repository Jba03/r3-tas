//
//  structure.h
//  r3-tas-structure
//
//  Created by Jba03 on 2022-12-02.
//

#ifndef structure_h
#define structure_h

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct structure_info {
    const char* name;
    address offset;
};

struct structure_entry_data {
    address offset;
    void* contents;
};

struct structure_entry {
    int type;
    const char* name;
    struct structure_entry_data data;
};

struct structure {
    struct structure_info info;
    struct structure_entry entries[];
};

#define s_type_null    -1
#define s_type_int8     0
#define s_type_uint8    1
#define s_type_int16    2
#define s_type_uint16   3
#define s_type_int32    4
#define s_type_uint32   5
#define s_type_int64    6
#define s_type_uint64   7
#define s_type_float    8
#define s_type_double   9
#define s_type_pointer  10
#define s_type_string   11
#define s_type_padding  12

#define s_struct_fix            16
#define s_struct_lvl            16
#define s_struct_engine         16
#define s_struct_engine_timer   17
#define s_struct_superobject    17
#define s_struct_physicalobject 18
#define s_struct_ipo            18
#define s_struct_ipo2            18
#define s_struct_actor
#define s_struct_stdgame
#define s_struct_dynamics
#define s_struct_brain
#define s_struct_mind
#define s_struct_intelligence
#define s_struct_behavior
#define s_struct_dsg
#define s_struct_script

static const char * const s_name[] =
{
    "int8", "uint8", "int16", "uint16", "int32", "uint32",
    "int64", "uint64", "float", "double", "pointer", "string", "string",
    "FIX", "LVL", "Engine", "Engine timer", "Superobject", "Physical object",
    "IPO", "IPO.2", "Actor", "StdGame", "Dynamics", "Brain", "Mind", "Intelligence",
    "Behavior", "DSG", "Script",
};

static struct structure* structure_list[32];

static inline struct structure_entry structure_get_entry(struct structure* s, const char* name)
{
    struct structure_entry* p = s->entries;
    while (p->type != s_type_null && p->name != NULL)
    {
        if (!strcmp(p->name, name)) return *p;
        p++;
    }
}

#define structure_register(type, S) structure_list[type - s_struct_fix] = S;

#define structure_begin {},
#define structure_end {s_type_null, NULL}

#define structure_get(S, field) structure_get_entry(S, field).data.contents
#define structure_get_offset(S, field) structure_get_entry(S, field).data.offset

#define structure_get_int8(S, field)    (*((int8_t*)structure_get(S, field)))
#define structure_get_uint8(S, field)   (*((uint8_t*)structure_get(S, field)))
#define structure_get_int16(S, field)   (*((int16_t*)structure_get(S, field)))
#define structure_get_uint16(S, field)  (*((uint16_t*)structure_get(S, field)))
#define structure_get_int32(S, field)   (*((int32_t*)structure_get(S, field)))
#define structure_get_uint32(S, field)  (*((uint32_t*)structure_get(S, field)))
#define structure_get_float(S, field)   (*((float*)structure_get(S, field)))
#define structure_get_double(S, field)  (*((double*)structure_get(S, field)))
#define structure_get_float(S, field)   (*((float*)structure_get(S, field)))

#endif /* structure_h */
