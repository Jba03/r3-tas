#ifndef stCollideElementSpheres_h
#define stCollideElementSpheres_h

typedef struct stCollideElementIndexedSphere stCollideElementIndexedSphere;
typedef struct stCollideElementSpheres stCollideElementSpheres;

struct stCollideElementIndexedSphere
{
    readonly float32 radius;
    readonly pointer<> material;
    readonly int16 center;
    readonly padding(2)
};

struct stCollideElementSpheres
{
    readonly pointer<> spheres; /* ::stCollideElementIndexedSphere */
    readonly int16 numSpheres;
    readonly int16 aabbIndex;
};

#endif /* stCollideElementSpheres_h */
