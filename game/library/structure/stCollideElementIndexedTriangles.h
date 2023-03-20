#ifndef stCollideElementIndexedTriangles_h
#define stCollideElementIndexedTriangles_h

#include "structure.h"

struct stElementIndexedTrianglesVisual {
    readonly pointer visual_material;
    readonly int16 n_faces;
    readonly int16 n_uvs;
    readonly int16 n_uv_stages;
    padding(2)
    readonly pointer face_indices;
    readonly pointer face_indices_uv;
    readonly pointer face_normals;
    readonly pointer uv_elements;
    readonly pointer edges;
    readonly pointer adjacent_faces;
    readonly pointer this_indexlist;
    readonly int16 n_used_indices;
    readonly int16 bbox_index;
    readonly uint32 displaylist;
    readonly pointer unknown;
    readonly uint8 portalvisibility;
    padding(3)
    readonly uint32 vao[4];
} typedef tdstElementIndexedTrianglesVisual;

struct stCollideElementIndexedTriangles {
    readonly pointer material;
    readonly pointer face_indices;
    readonly pointer normals;
    readonly int16 n_faces;
    readonly int16 aabb_index;
    readonly pointer geo_visualization; /* above struct */
    readonly pointer face_edges;
    readonly pointer edge_normals;
    readonly pointer edge_coefficients;
    readonly int16 n_edges;
    padding(2)
} typedef tdstCollideElementIndexedTriangles;

#endif /* stCollideElementIndexedTriangles_h */
