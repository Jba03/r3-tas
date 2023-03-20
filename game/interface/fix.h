#ifndef fix_h
#define fix_h

#include "structure.h"

struct fix_header
{
    padding(32)
    readonly pointer identity_matrix;
    readonly pointer localization_structure;
    readonly uint32 level_name_count;
    readonly uint32 demo_name_count;
};

struct fix_trailer
{
    readonly char8 first_level[30];
    padding(2)
    readonly uint32 language_count;
    readonly uint32 language_offset;
    readonly uint32 texture_count;
};

struct fix
{
    readonly struct fix_header* header;
    readonly struct fix_trailer* trailer;
};

#endif /* fix_h */
