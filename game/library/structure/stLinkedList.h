#ifndef stLinkedList_h
#define stLinkedList_h

#include "structure.h"

typedef struct stLinkedList tdstLinkedList;
typedef struct stDoublyLinkedList tdstDoublyLinkedList;

struct stLinkedList
{
    readonly pointer first;
    readonly int32 numEntries;
};

struct stDoublyLinkedList
{
    readonly pointer first;
    readonly pointer last;
    readonly int32 numEntries;
};

#endif /* stLinkedList_h */
