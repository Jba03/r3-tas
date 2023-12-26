#ifndef stCollideElementSpheres_h
#define stCollideElementSpheres_h

#include "library.hh"

struct stCollideElementIndexedSphere {
  readonly float32 radius;
  readonly pointer<> material;
  readonly int16 indexOfCenterPoint;
  readonly padding(2)
};

struct stCollideElementSpheres {
  readonly pointer<stCollideElementIndexedSphere> spheres;
  readonly int16 numSpheres;
  readonly int16 aabbIndex;
};

#endif /* stCollideElementSpheres_h */
