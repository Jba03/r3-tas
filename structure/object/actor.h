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

#define ACTOR

struct Actor {
    
    struct StandardGameInfo* info;
    /* struct Dynamics dynamics; */
    /* struct Brain brain; */
    
    pointer data3d;
    pointer camera;
    pointer collision_set;
    pointer ms_way;
    pointer light;
    pointer sector_info;
    
    const char* family_name;
    const char* model_name;
    const char* instance_name;
    
    /* Containing superobject */
    struct SuperObject* superobject;
};

ACTOR struct Actor *actor_read(const address addr);

/* actor_find: Try to find an actor by its instance name in the hierarchy of specified superobject. */
ACTOR struct Actor *actor_find(const char* name, const struct SuperObject* superobject);

#endif /* actor_h */
