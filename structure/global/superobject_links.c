//
//  sol_links.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "superobject_links.h"

SUPEROBJECT_LINKS struct SuperObjectLinks *superobject_links_read(struct Stream *stream) {
    uint64_t checkpoint = stream->position;
    
    struct SuperObjectLinks* sol = (struct SuperObjectLinks*)malloc(sizeof *sol);
    stream_seek(stream, readpointer());
    sol->superobject = superobject_read(stream);
    
    /* Seek to after SO pointer */
    stream_seek(stream, checkpoint + 4);
    
    sol->transform_initial = transform_read(stream);
    sol->n_linked_objects = read32();
    sol->n_links_allocated = read32();
    readpointer();
    
    //sol->linked_object = (void**)readpointer();
    
    /* Read object type at pointer */
    checkpoint = stream->position;
    stream_seek(stream, readpointer());
    sol->obj_type = (enum SuperObjectLinkType)read32();
    stream_seek(stream, checkpoint + 4);
    
    
    /* Read initial position at pointer */
    checkpoint = stream->position;
    stream_seek(stream, readpointer());
    sol->obj_type = (enum SuperObjectLinkType)read32();
    stream_seek(stream, checkpoint + 4);
    
    
    return sol;
}
