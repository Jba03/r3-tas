#ifndef stInstantiatedPhysicalObject_h
#define stInstantiatedPhysicalObject_h

#include "library.hh"
#include "stPhysicalObject.hh"
#include "stCollideObject.hh"

typedef struct stInstantiatedPhysicalObject
{
    readonly pointer<stPhysicalObject> physicalObject;
    readonly pointer<> currentRadiosity;
    readonly doublepointer<> radiosity;
    readonly pointer<> portalCamera;
    readonly uint32 lastTransitionID;
    readonly float32 lastRatioUsed;
#if (platform == platformGCN)
    readonly padding(4) /* Unknown? */
    readonly char8 name[0x32];
    
    std::string getName()
    {
        std::string s = reinterpret_cast<char*>(name);
        return s.substr(s.rfind(':') + 1 , std::string::npos);
    }
#endif
    
    stCollideObject *zdr()
    {
        stCollideObject *zdr = nullptr;
        if (physicalObject)
            if (physicalObject->physicalCollideset)
                zdr = physicalObject->physicalCollideset->zdr;
        
        return zdr;
    }
} stIPO;

#endif /* stInstantiatedPhysicalObject_h */
