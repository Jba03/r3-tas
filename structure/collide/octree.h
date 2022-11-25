//
//  octree.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#ifndef octree_h
#define octree_h

#include "vector3.h"

#define OCTREE

struct OctreeNode {
    pointer children_ptr;
    pointer face_indices_ptr;
    
    struct Vector3 min;
    struct Vector3 max;
    struct OctreeNode** children;
    uint8_t* face_indices;
    
    /* not part of struct */
    int n_children;
    int n_face_indices;
    
    address offset;
};

struct Octree {
    pointer root_node_ptr;
    pointer element_base_table_ptr;
    
    struct OctreeNode* root;
    struct Vector3 min;
    struct Vector3 max;
    uint16_t* element_base_table;
    uint16_t n_faces;
    
    address offset;
};

OCTREE struct Octree* octree_read(const address addr);

#endif /* octree_h */
