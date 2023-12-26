#ifndef stCollideSet_h
#define stCollideSet_h

#include "stLinkedList.hh"
#include "stCollideObject.hh"

struct stPhysicalCollideSet {
  readonly pointer<stCollideObject> zdm;
  readonly pointer<stCollideObject> zdd;
  readonly pointer<stCollideObject> zde;
  readonly pointer<stCollideObject> zdr;
};

struct stColliderInfo {
  readonly pointer<> colliderActors[2]; /* ::so */
  readonly stVector3D colliderVectors[2];
  readonly float32 colliderReal[2];
  readonly uint8 colliderType;
  readonly uint8 colliderPriority;
  readonly uint8 unused[2];
};

struct stZdxListEntry {
  readonly pointer<> next;
  readonly pointer<> prev;
  readonly pointer<> parent;
  readonly pointer<> data;
};

struct stZdxList {
  readonly stDoublyLinkedList<stZdxListEntry> list;
  readonly uint16 numZdx;
  readonly padding(2)
  
  template <typename F>
  void forEachElement(const F& f) {
    list.forEach([&](stZdxListEntry *entry, void*) {
      if (entry) f(entry->data);
    });
  }
};

struct stCsaList {
  readonly stDoublyLinkedList<> list;
};

struct stCollideSet {
  readonly pointer<stZdxList> zddList;
  readonly pointer<stZdxList> zdeList;
  readonly pointer<stZdxList> zdmList;
  readonly pointer<stZdxList> zdrList;
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

#endif /* stCollideSet_h */
