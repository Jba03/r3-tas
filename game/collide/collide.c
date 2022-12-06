//
//  collide.c
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#include "collide.h"
#include "mesh.h"
#include "common.h"
#include "octree.h"

COLLIDE struct CollisionGeometry *collision_geometry_read(const address address)
{
    struct CollisionGeometry *geom = malloc(sizeof *geom);
    geom->offset = address;
    geom->octree = NULL;
    
    struct Stream *stream = stream_open(address);
    geom->n_vertices = read16();
    geom->n_elements = read16();
    geom->n_aabb = read16();
    advance(2); /* Padding x 2 */
    geom->vertex_offset = readpointer();
    geom->element_types_offset = readpointer();
    geom->element_offset = readpointer();
    geom->octree_offset = readpointer();
    geom->aabb_offset = readpointer();
    
    geom->bounding_sphere.radius = readfloat();
    geom->bounding_sphere.center = vector4_read(stream->position);
    
    
    geom->vertices = malloc(sizeof(struct Vector3) * geom->n_vertices);
    geom->element_types = malloc(sizeof(uint16_t) * geom->n_elements);
    geom->elements = malloc(sizeof(void*) * geom->n_elements);
    
    //info("Geometry (%d vertices)\n", geom->n_vertices);
    
    /* Read vertices */
    stream_seek(stream, geom->vertex_offset);
    for (unsigned n = 0; n < geom->n_vertices; n++)
    {
        struct Vector3* vertex = &geom->vertices[n];
        vertex->offset = stream->position;
        vertex->x = readfloat();
        vertex->z = readfloat();
        vertex->y = readfloat();
    }
    
    /* Read element types */
    stream_seek(stream, geom->element_types_offset);
    for (unsigned e = 0; e < geom->n_elements; e++)
        geom->element_types[e] = read16();
    
    /* Read elements */
    for (unsigned i = 0; i < geom->n_elements; i++)
    {
        stream_seek(stream, geom->element_offset + 4 * i);
        pointer block = readpointer();
        uint16_t type = geom->element_types[i];
        
        switch (type)
        {
            case 1: /* Indexed triangles */
                geom->elements[i] = mesh_read(block, geom);
                mesh_reindex(geom->elements[i]);
                mesh_create_glmesh(geom->elements[i]);
                break;
                
            case 2: /* Facemap */
                break;
                
            case 3: /* Sprite */
                break;
                
            case 4: /* TMesh */
                break;
                
            case 5: /* Points */
                break;
                
            case 6: /* Lines */
                break;
                
            case 7: /* Spheres */
                break;
                
            case 8: /* AABB */
                break;
                
            case 9: /* Cones */
                break;
                
            case 13: /* Deformation set info */
                break;
                
            default:
                break;
        }
    }
    stream_close(stream);
    
    /* Read octree */
    if (geom->octree_offset != 0x00)
        geom->octree = octree_read(geom->octree_offset, geom);
    
    return geom;
}

COLLIDE void collision_geometry_free(struct CollisionGeometry *geom)
{
    //info(COLOR_GREEN "free CollisionGeometry[%X]\n", geom->offset);
    
    for (int i = 0; i < geom->n_elements; i++)
    {
        uint16_t type = geom->element_types[i];
        if (type == 1) mesh_destroy(geom->elements[i]);
    }
    
    free(geom->vertices);
    free(geom->element_types);
    free(geom->elements);
    free(geom);
}
