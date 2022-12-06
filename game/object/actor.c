//
//  actor.c
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#include "actor.h"
#include "brain.h"

ACTOR struct Actor *actor_read(const address addr)
{
    struct Actor *actor = malloc(sizeof *actor);
    actor->offset = addr;
    actor->info = NULL;
    actor->dynamics = NULL;
    actor->dynamics_ptr = 0x00;
    actor->superobject = NULL;
    actor->brain = NULL;
    
    struct Stream *stream = stream_open(addr);
    actor->data3d_ptr = readpointer();
    actor->stdgame_ptr = readpointer();
    actor->dynam_ptr = readpointer();
    actor->brain_ptr = readpointer();
    advance(4);
    actor->camera_info_ptr = readpointer();
    actor->collision_set_ptr = readpointer();
    actor->ms_way_ptr = readpointer();
    advance(4);
    actor->sector_info_ptr = readpointer();
    
    if (actor->stdgame_ptr != 0x00) actor->info = stdgame_read(actor->stdgame_ptr);
    if (actor->brain_ptr != 0x00) actor->brain = brain_read(actor->brain_ptr);
    
    if (actor->dynam_ptr != 0x00)
    {
        stream_seek(stream, actor->dynam_ptr);
        pointer dynamics = readpointer();
        advance(0x4);
        int32_t mechanics = read32S();
        
        actor->dynamics_ptr = dynamics;
        actor->dynamics = dynamics_read(dynamics);
        actor->mechanics_used = mechanics;
    }
    
    if (actor->info)
    {
        actor->family_name = stdgame_family_name(actor->info);
        actor->model_name = stdgame_model_name(actor->info);
        actor->instance_name = stdgame_instance_name(actor->info);
        
        if (!strcmp(actor->family_name, "rayman"))
            rayman = actor;
        
        if (!strcmp(actor->family_name, "Camera"))
            camera_actor = actor;
        
        if (!strcmp(actor->family_name, "Reflux_1"))
            reflux = actor;
    }
    
    stream_close(stream);
    
    return actor;
}

/* TODO: fix crashes */
ACTOR struct Actor *actor_find(int mode, const char* name, const struct SuperObject* superobject)
{
    struct Actor* actor = superobject->data;
    if (superobject->type == SUPEROBJECT_TYPE_ACTOR && superobject->data_ptr != 0x00 && actor)
    {
        const char* compare = actor->instance_name;
        if (mode == ACTOR_FIND_MODEL) compare = actor->model_name;
        if (mode == ACTOR_FIND_FAMILY) compare = actor->family_name;
        if (strcmp(compare, name) == 0) return actor;
    }
        
    struct Actor* ret = NULL;
    if (superobject->child_first != 0x00 && superobject->child_last != 0x00)
        for (int n = 0; !ret && n < superobject->n_children; n++)
            ret = actor_find(mode, name, superobject->children[n]);
    
    return ret;
}

ACTOR void actor_free(struct Actor *act)
{
    stdgame_free(act->info);
    free(act);
}
