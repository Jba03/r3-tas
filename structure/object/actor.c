//
//  actor.c
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#include "actor.h"

ACTOR struct Actor *actor_read(const address addr)
{
    struct Actor *actor = malloc(sizeof *actor);
    actor->info = NULL;
    actor->superobject = NULL;
    
    struct Stream *stream = stream_open(addr);
    actor->data3d = readpointer();
    
    /* Read standard game info */
    uint32_t stdgame = readpointer();
    if (stdgame != 0x00) actor->info = stdgame_read(stdgame);
    
    if (actor->info)
    {
        actor->family_name = stdgame_family_name(actor->info);
        actor->model_name = stdgame_model_name(actor->info);
        actor->instance_name = stdgame_instance_name(actor->info);
    }
    
    stream_close(stream);
    
    return actor;
}

ACTOR struct Actor *actor_find(const char* name, const struct SuperObject* superobject)
{
    struct Actor* actor = superobject->data;
    if (superobject->type == SUPEROBJECT_TYPE_ACTOR && superobject->data_ptr != 0x00 && actor)
        if (strcmp(actor->instance_name, name) == 0) return actor;
        
    struct Actor* ret = NULL;
    for (int n = 0; !ret && n < superobject->n_children; n++)
        ret = actor_find(name, superobject->children[n]);
    
    return ret;
}

ACTOR void actor_free(struct Actor *act)
{
    stdgame_free(act->info);
    free(act);
}
