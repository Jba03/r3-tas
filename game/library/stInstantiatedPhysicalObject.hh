#ifndef stInstantiatedPhysicalObject_h
#define stInstantiatedPhysicalObject_h

#include "library.hh"

typedef struct stInstantiatedPhysicalObject stInstantiatedPhysicalObject, stIPO;

struct stInstantiatedPhysicalObject {
    readonly pointer<> stPhysicalObject;
    readonly pointer<> currentRadiosity;
    readonly doublepointer<> radiosity;
    readonly pointer<> portalCamera;
    readonly uint32 lastTransitionID;
    readonly float32 lastRatioUsed;
#if platform == GCN || platform == PC
    readonly padding(4) /* Unknown? */
    readonly char8 name[0x32];
    
    std::string getName() {
        std::string s = reinterpret_cast<char*>(name);
        return s.substr(s.rfind(':') + 1 , std::string::npos);
    }
#endif
};

#ifdef USE_FUNCTIONS

#include "stSuperObject.hh"
#include "stCollideObject.hh"

/** fnIPOGetName: get the name of an IPO */
const char* fnIPOGetName(const stInstantiatedPhysicalObject* ipo);

/* ipo_world_matrix: compute the world transform of an IPO */
const stMatrix4D ipo_world_matrix(const stSuperObject* ipo);

/** fnIPOGetCollideObject: get the collide object (ZDR) of an IPO */
stCollideObject* fnIPOGetCollideObject(const stInstantiatedPhysicalObject* ipo);

stInstantiatedPhysicalObject* fnFindIPO(const char* name, stSuperObject *root, stSuperObject** so);

#endif
#endif /* stInstantiatedPhysicalObject_h */
