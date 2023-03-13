#ifndef collide_spheres_h
#define collide_spheres_h

struct indexed_sphere
{
    readonly float32 radius;
    readonly pointer material;
    readonly int16 center;
    padding(2)
};

struct collide_element_spheres
{
    readonly pointer spheres; /* ::indexed_sphere */
    readonly int16 n_spheres;
    readonly int16 aabb_index;
};

#endif /* collide_spheres_h */
