//
//  collide_object.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-05.
//

#include "collide_object.h"
#include "game.h"

/** collide_object_mesh: get the collide mesh at specified index */
const struct collide_mesh* collide_object_mesh(const struct collide_object* object, int idx)
{
    if (!object) return NULL;
    
    int counter = 0;
    for (int i = 0; i < host_byteorder_16(object->n_elements); i++)
    {
        const pointer element = (*((pointer*)pointer(object->elements) + i));
        const int16_t type = host_byteorder_16(*((int16_t*)pointer(object->element_types) + i));
        const void* block = pointer(element);
        
        if (type == collide_object_indexed_triangles)
            if (counter++ == idx) return block;
    }
    
    return NULL;
}

/** collide_object_sphere: get collide sphere at specified index */
const struct collide_mesh* collide_object_sphere(const struct collide_object* object, int idx)
{
    if (!object) return NULL;
    
    int counter = 0;
    for (int i = 0; i < host_byteorder_16(object->n_elements); i++)
    {
        const pointer element = (*((pointer*)pointer(object->elements) + i));
        const int16_t type = host_byteorder_16(*((int16_t*)pointer(object->element_types) + i));
        const void* block = pointer(element);
        
        if (type == collide_object_spheres)
            if (counter++ == idx) return block;
    }
    
    return NULL;
}
