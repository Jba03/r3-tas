#ifndef stObjectType_h
#define stObjectType_h

#include "stLinkedList.hh"
#include <string>

#define object_family_name      0
#define object_model_name       1
#define object_instance_name    2

struct stObjectTypeElement {
    readonly pointer<> next;
    readonly pointer<> prev;
    readonly pointer<> father;
    readonly pointer<> name;
    readonly uint8 priority;
    readonly uint8 identifier;
    readonly padding(2)
};

struct stObjectType {
    readonly stDoublyLinkedList<> family;
    readonly stDoublyLinkedList<> model;
    readonly stDoublyLinkedList<> instance;
    
    std::string getName(int kind, int idx) {
        stDoublyLinkedList<>& list = family;
        switch (kind) {
            case object_family_name: list = family;
            case object_model_name: list = model;
            case object_instance_name: list = instance;
        }
        tryAgain:
        if (!list.first.offset() || !list.last.offset())
            return std::string("Invalid name list");
        
        stObjectTypeElement *T = list.first;
        for (unsigned int i = 0; T && i < idx; ++i)
            T = T->next;
        
        return T ? std::string(T->name) : std::string();
    }
};

#endif /* stObjectType_h */
