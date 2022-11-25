//
//  octree.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#include "octree.h"
#include "stream.h"

OCTREE struct OctreeNode* octree_node_read(const address addr)
{
    struct OctreeNode* node = malloc(sizeof *node);
    node->offset = addr;
    
    struct Stream* stream = stream_open(addr);
    node->min = vector3_read(stream->position + 0 * 3 * 4);
    node->max = vector3_read(stream->position + 1 * 3 * 4);
    advance(2 * 3 * 4); /* Advance past vector offsets */
    node->child_list_ptr = readpointer();
    node->face_indices_ptr = readpointer();
    
//    info("Octree node @ %X: children @ %X, face indices @ %X\n", addr, node->child_list_ptr, node->face_indices_ptr);
//    info("\tmin: %f %f %f\n", node->min.x, node->min.y, node->min.z);
//    info("\tmax: %f %f %f\n", node->max.x, node->max.y, node->max.z);
    
    if (node->child_list_ptr != 0x00)
    {
        stream_seek(stream, node->child_list_ptr);
        for (unsigned i = 0; i < 8; i++)
        {
            uint32_t child_offset = readpointer();
            node->children[i] = octree_node_read(child_offset);
        }
    }
    
    stream_close(stream);
    
    return node;
}

OCTREE struct Octree* octree_read(const address addr)
{
    struct Octree* octree = malloc(sizeof *octree);
    octree->offset = addr;
    octree->root = NULL;
    
    struct Stream* stream = stream_open(addr);
    octree->root_node_ptr = readpointer();
    octree->n_faces = read16();
    advance(0x2); /* Padding */
    octree->element_base_table_ptr = readpointer();
    octree->min = vector3_read(stream->position + 0 * 3 * 4);
    octree->max = vector3_read(stream->position + 1 * 3 * 4);
    stream_close(stream);
    
    if (octree->root_node_ptr != 0x00)
        octree->root = octree_node_read(octree->root_node_ptr);
    
//    info("Octree @ %X: %d faces, elements @ %X\n", addr, octree->n_faces, octree->element_base_table_ptr);
//    info("\tmin: %f %f %f\n", octree->min.x, octree->min.y, octree->min.z);
//    info("\tmax: %f %f %f\n", octree->max.x, octree->max.y, octree->max.z);
    
    return octree;
}
