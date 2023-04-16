#ifndef stCollideElementSpheres_h
#define stCollideElementSpheres_h

typedef struct stIndexedSphere tdstIndexedSphere;
typedef struct stCollideElementSpheres tdstCollideElementSpheres;

struct stIndexedSphere
{
    readonly float32 radius;
    readonly pointer material;
    readonly int16 center;
    readonly padding(2)
};

struct stCollideElementSpheres
{
    readonly pointer spheres; /* ::tdstIndexedSphere */
    readonly int16 numSpheres;
    readonly int16 aabbIndex;
};

#endif /* stCollideElementSpheres_h */
