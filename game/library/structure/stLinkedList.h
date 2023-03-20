#ifndef stLinkedList_h
#define stLinkedList_h

#include "structure.h"

struct stLinkedList {
    readonly pointer first;
    readonly int32 n_entries;
} typedef tdstLinkedList;

struct stDoublyLinkedList {
    readonly pointer first;
    readonly pointer last;
    readonly int32 n_entries;
} typedef tdstDoublyLinkedList;

#endif /* stLinkedList_h */
