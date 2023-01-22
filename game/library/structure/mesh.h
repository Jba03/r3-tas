#ifndef mesh_h
#define mesh_h

#include <stdio.h>
#include <stdbool.h>

#include "vertex.h"
#include "collide.h"
#include "material.h"

#define MESH

struct Mesh {
    pointer material_offset;
    pointer triangles_offset;
    pointer normals_offset;
    pointer mapping_offset;
    
    uint16_t aabb_index;
    uint16_t mapping_count;
    uint16_t n_triangles;
    uint16_t n_edges;
    
    struct vector3* edge_normals;
    float* edge_coefficients;
    
    struct {
        struct vector3* vertices;
        struct vector3* normals;
        uint16_t* indices;
    } original, processed;
    
    struct CollisionMaterial material;
    
    bool was_processed;
    struct GLMesh* glmesh;
    
    address offset;
};

#endif /* mesh_h */
