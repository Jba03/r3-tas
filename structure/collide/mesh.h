//
//  mesh.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef mesh_h
#define mesh_h

#include <stdio.h>
#include <stdbool.h>

#include "vertex.h"
#include "collide.h"
#include "glmesh.h"

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
    
    union Vector3* edge_normals;
    float* edge_coefficients;
    
    struct {
        union Vector3* vertices;
        union Vector3* normals;
        uint16_t* indices;
    } original, processed;
    
    bool was_processed;
    struct GLMesh* glmesh;
    
    address offset;
};

MESH struct Mesh* mesh_read(const address addr, struct CollisionGeometry* geom);

MESH void mesh_reindex(struct Mesh* mesh);

MESH void mesh_create_glmesh(struct Mesh* mesh);

#endif /* mesh_h */
