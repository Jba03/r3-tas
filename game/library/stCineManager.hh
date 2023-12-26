#ifndef stCineManager_hh
#define stCineManager_hh

#include "library.hh"
#include "stLinkedList.hh"

struct stCine {
  
  auto getName() -> std::string {
    return name;
  }
  
  readonly stDoublyLinkedList<stEngineObject> actors;
  readonly pointer<stCine> next;
  readonly pointer<stCine> prev;
  readonly pointer<stDoublyLinkedList<stCine>> parent;
  readonly uint8 playing;
  readonly padding(3)
  readonly uint32 event;
  readonly string<255> name;
};

struct stCineManager {
  readonly stDoublyLinkedList<stCine> cineList;
  readonly stTransform fixedCameraTransform;
  readonly pointer<stSuperObject> camera;
};

#endif /* stCineManager_hh */
