#ifndef stLinkedList_h
#define stLinkedList_h

#include "library.hh"

template <typename T = uint32_t>
struct stLinkedList {
  readonly pointer<T> first;
  readonly int32 numEntries;
};

template <typename T = uint32_t>
struct stDoublyLinkedList {
  readonly pointer<T> first;
  readonly pointer<T> last;
  readonly int32 numEntries;
  
  template <typename F>
  void forEach(const F& f, void *userdata = nullptr) {
    for (T *c = first; c; c = c->next) {
      f(c, userdata);
    }
  }
};

#endif /* stLinkedList_h */
