#ifndef stSector_h
#define stSector_h

#include "library.hh"
#include "stLinkedList.hh"
#include "stVector3D.hh"

#define sector_priority_min     0
#define sector_priority_normal  64
#define sector_priority_max     127

struct stSector {
    readonly stDoublyLinkedList characterList;
    readonly stDoublyLinkedList staticLightList;
    readonly stDoublyLinkedList dynamicLightList;
    readonly stDoublyLinkedList graphicSectorList;
    readonly stDoublyLinkedList collisionSectorList;
    readonly stDoublyLinkedList activitySectorList;
    readonly stDoublyLinkedList soundSectorList;
    readonly stDoublyLinkedList soundEventList;
    readonly stVector3D min;
    readonly stVector3D max;
    readonly float32 farPlane;
    readonly uint8 isVirtual;
    readonly int8 cameraType;
    readonly int8 counter;
    readonly int8 priority;
    readonly pointer<> skyMaterial;
    readonly uint8 fog;
#if platform == GCN
    readonly char8 name[0x104];
#endif
    
    template <bool fullName=false> std::string getName() {
        std::string str = reinterpret_cast<const char*>(name);
        return fullName ? str : str.substr(str.rfind(':')+1, std::string::npos);
    }
};

#endif /* stSector_h */
