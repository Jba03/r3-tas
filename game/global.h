//
//  global.h
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef global_h
#define global_h

#include "common.h"
#include "array.h"
#include "stream.h"
#include "logging.h"
#include "camera.h"
#include "actor.h"
#include "random.h"
#include "graph.h"

extern struct FIX* fix;
extern struct LVL* lvl;
extern struct Engine* engine;
extern struct RandomNumberDevice rng_device;

extern struct Camera* camera;

extern struct Actor* rayman;
extern struct Actor* camera_actor;
extern struct Actor* reflux;

extern int log_indent;

extern int family_colors;
extern uint32_t family_color_table[1000];


extern struct Array* graph_list;

#endif /* global_h */
