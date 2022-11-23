//
//  collide.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef collide_h
#define collide_h

#include "vector3.h"
#include "vector4.h"

#define COLLIDE

struct CollideSet {
    
};

struct CollisionGeometry {
    pointer vertex_offset;
    pointer element_types_offset;
    pointer element_offset;
    pointer aabb_offset;
    
    uint16_t n_vertices;
    uint16_t n_elements;
    uint16_t n_aabb;
    
    union Vector3* vertices;
    uint16_t* element_types;
    void** elements;
    
    struct {
        float radius;
        union Vector4 center;
    } bounding_sphere;
    
    address offset;
};

COLLIDE struct CollisionGeometry *collision_geometry_read(const uint32_t address);

#endif /* collide_h */
