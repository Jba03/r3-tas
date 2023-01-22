//
//  collide_mesh.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-01-09.
//

#ifndef collide_mesh_h
#define collide_mesh_h

#include "structure.h"

struct collide_mesh_visual
{
    readonly pointer visual_material;
    readonly int16_t n_faces;
    readonly int16_t n_uvs;
    readonly int16_t n_uv_stages;
    padding(2)
    readonly pointer face_indices;
    readonly pointer face_indices_uv;
    readonly pointer face_normals;
    readonly pointer uv_elements;
    readonly pointer edges;
    readonly pointer adjacent_faces;
    readonly pointer this_indexlist;
    readonly int16_t n_used_indices;
    readonly int16_t bbox_index;
    readonly uint32_t displaylist;
    readonly pointer unknown;
    readonly uint8_t portalvisibility;
    padding(3)
    readonly uint32_t vao[4];
};

struct collide_mesh
{
    readonly pointer material;
    readonly pointer face_indices;
    readonly pointer normals;
    readonly int16_t n_faces;
    readonly int16_t aabb_index;
    readonly pointer geo_visualization; /* above struct */
    readonly pointer face_edges;
    readonly pointer edge_normals;
    readonly pointer edge_coefficients;
    readonly int16_t n_edges;
    padding(2)
};

#endif /* collide_mesh_h */
