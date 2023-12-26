#ifndef stLinkedList_h
#define stLinkedList_h

struct stLinkedList
{
    readonly pointer<> first;
    readonly int32 numEntries;
};

struct stDoublyLinkedList
{
    readonly pointer<> first;
    readonly pointer<> last;
    readonly int32 numEntries;
};

#endif /* stLinkedList_h */
