#ifndef stCollideElementSpheres_h
#define stCollideElementSpheres_h

struct stIndexedSphere {
    readonly float32 radius;
    readonly pointer material;
    readonly int16 center;
    padding(2)
} typedef tdstIndexedSphere;

struct stCollideElementSpheres {
    readonly pointer spheres; /* ::tdstIndexedSphere */
    readonly int16 n_spheres;
    readonly int16 aabb_index;
} typedef tdstCollideElementSpheres;

#endif /* stCollideElementSpheres_h */
