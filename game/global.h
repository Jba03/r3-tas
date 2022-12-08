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

#define GCN_POINTER_FIX         0x004334cc /* GCN location of FIX */
#define GCN_POINTER_LVL         0x004334d0 /* GCN location of LVL */
#define GCN_POINTER_ENGINE      0x003E7C0C /* GCN location of engine structure */
#define GCN_POINTER_HIERARCHY1  0x004334d4 /* GCN location of superobject hierarchy pointer 1 */
#define GCN_POINTER_HIERARCHY2  0x004334d8 /* GCN location of superobject hierarchy pointer 2 */
#define GCN_POINTER_RND         0x00436924 /* GCN location of random number device */

extern struct FIX* fix;
extern struct LVL* lvl;
extern struct Engine* engine;
extern struct RandomNumberDevice rnd;

extern struct Camera* camera;

extern struct Actor* rayman;
extern struct Actor* camera_actor;
extern struct Actor* reflux;

extern int log_indent;

extern int family_colors;
extern uint32_t family_color_table[1000];


extern struct Array* actor_list;
extern struct Array* graph_list;

#endif /* global_h */
