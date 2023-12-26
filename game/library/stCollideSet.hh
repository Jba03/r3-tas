#ifndef stCollideSet_h
#define stCollideSet_h

#include "stLinkedList.hh"

typedef struct stPhysicalCollideSet stPhysicalCollideSet;
typedef struct stColliderInfo stColliderInfo;
typedef struct stCollideSet stCollideSet;
typedef struct stZdxList stZdxList;
typedef struct stZdxListEntry stZdxListEntry;

struct stPhysicalCollideSet
{
    readonly pointer<> zdm;
    readonly pointer<> zdd;
    readonly pointer<> zde;
    readonly pointer<> zdr;
};

struct stColliderInfo
{
    readonly pointer<> colliderActors[2]; /* ::so */
    readonly stVector3D colliderVectors[2];
    readonly float32 colliderReal[2];
    readonly uint8 colliderType;
    readonly uint8 colliderPriority;
    readonly uint8 unused[2];
};

struct stCollideSet
{
    readonly pointer<> zddList;
    readonly pointer<> zdeList;
    readonly pointer<> zdmList;
    readonly pointer<> zdrList;
    readonly pointer<> zddActivationList;
    readonly pointer<> zdeActivationList;
    readonly pointer<> zdmActivationList;
    readonly pointer<> zdrActivationList;
    readonly pointer<> zddCurrentActivation;
    readonly pointer<> zdeCurrentActivation;
    readonly pointer<> zdrCurrentActivation;
    readonly pointer<> zdmCurrentActivation;
    readonly uint32 zddPrivilegedZone;
    readonly uint32 zdePrivilegedZone;
    readonly uint32 zdmPrivilegedZone;
    readonly uint32 zdrPrivilegedZone;
    readonly uint8 computeFrequency;
    readonly uint8 characterPriority;
    readonly uint8 collisionFlag;
    readonly padding(1)
    readonly stColliderInfo colliderInfo;
};

struct stZdxList
{
    readonly stDoublyLinkedList list;
    readonly uint16 numZdx;
    readonly padding(2)
};

struct stZdxListEntry
{
    readonly pointer<> next;
    readonly pointer<> prev;
    readonly pointer<> parent;
    readonly pointer<> data;
};

struct stCsaList
{
    readonly stDoublyLinkedList list;
};

#endif /* stCollideSet_h */
