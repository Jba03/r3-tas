#ifndef list_h
#define list_h

#include "structure.h"

typedef struct linked_list
{
    readonly pointer first;
    readonly int32_t n_entries;
} linked_list;

typedef struct doubly_linked_list
{
    readonly pointer first;
    readonly pointer last;
    readonly int32_t n_entries;
} doubly_linked_list;

#endif /* list_h */
