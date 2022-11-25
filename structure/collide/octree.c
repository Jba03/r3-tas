//
//  octree.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#include "octree.h"
#include "stream.h"

OCTREE struct Octree* octree_read(const address addr)
{
    struct Octree* octree = malloc(sizeof *octree);
    
    struct Stream* stream = stream_open(addr);
    octree->root_node_ptr = readpointer();
    octree->n_faces = read16();
    advance(0x2); /* Padding */
    octree->element_base_table_ptr = readpointer();
    octree->min = vector3_read(stream->position + 0 * 3 * 4);
    octree->max = vector3_read(stream->position + 1 * 3 * 4);
    stream_close(stream);
    
    info("Octree @ %X: %d faces, elements @ %X\n", addr, octree->n_faces, octree->element_base_table_ptr);
    info("\tmin: %f %f %f\n", octree->min.x, octree->min.y, octree->min.z);
    info("\tmax: %f %f %f\n", octree->max.x, octree->max.y, octree->max.z);
    
    return octree;
}
