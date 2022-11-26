//
//  actor.h
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#ifndef actor_h
#define actor_h

#include "stream.h"
#include "stdgame.h"
#include "dynamics.h"

#define ACTOR

#define ACTOR_MECHANICS_ERROR  -1
#define ACTOR_MECHANICS_CAMERA  0
#define ACTOR_MECHANICS_BASE    1

struct Actor {
    pointer data3d_ptr;
    pointer stdgame_ptr;
    pointer dynam_ptr;
    pointer dynamics_ptr;
    pointer brain_ptr;
    pointer camera_info_ptr;
    pointer collision_set_ptr;
    pointer ms_way_ptr;
    pointer light_ptr;
    pointer sector_info_ptr;
    
    struct StandardGameInfo* info;
    struct Dynamics* dynamics;
    int32_t mechanics_used;
    struct Brain* brain;
    
    const char* family_name;
    const char* model_name;
    const char* instance_name;
    
    /* Containing superobject */
    struct SuperObject* superobject;
    
    address offset;
};

ACTOR struct Actor *actor_read(const address addr);

/* actor_find: Try to find an actor by its instance name in the hierarchy of specified superobject. */
ACTOR struct Actor *actor_find(const char* name, const struct SuperObject* superobject);

ACTOR void actor_free(struct Actor *act);

#endif /* actor_h */
