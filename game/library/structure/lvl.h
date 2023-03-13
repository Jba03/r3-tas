#ifndef lvl_h
#define lvl_h

#include "list.h"
#include "object_type.h"
#include "always.h"

struct lvl_header
{
    padding(4 * 4) /* ? */
    readonly char8 text[24];
    padding(4 * 60) /* ? */
    readonly uint32 texture_count;
};

struct lvl_section_a
{
    readonly pointer actual_world;
    readonly pointer dynamic_world;
    readonly pointer inactive_dynamic_world;
    readonly pointer father_sector;
    readonly pointer first_submap_position;
    readonly struct always always_structure;
    readonly struct object_type_structure object_type;
};

struct lvl
{
    readonly struct lvl_header* header;
    readonly struct lvl_section_a* section_a;
};

#endif /* lvl_h */
