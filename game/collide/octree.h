//
//  octree.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#ifndef octree_h
#define octree_h

#include "collide.h"
#include "vector3.h"
#include "vector4.h"

#define OCTREE

struct OctreeNode {
    pointer child_list_ptr;
    pointer face_indices_ptr;
    
    struct Vector3 min;
    struct Vector3 max;
    struct OctreeNode* children[8];
    
    uint8_t n_face_indices;
    uint8_t face_indices[512];
    
    address offset;
};

struct Octree {
    pointer root_node_ptr;
    pointer element_base_table_ptr;
    
    struct OctreeNode* root;
    struct Vector3 min;
    struct Vector3 max;
    
    uint32_t n_base_elements;
    uint16_t* element_base_table;
    /* Number of targeted faces */
    uint16_t n_faces;
    
    address offset;
};

OCTREE struct OctreeNode* octree_node_read(const address addr);

OCTREE struct Octree* octree_read(const address addr, struct CollisionGeometry* geom);

OCTREE struct OctreeNode* octree_intersect_point(struct OctreeNode* root, struct Vector3 point);

#endif /* octree_h */