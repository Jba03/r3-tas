//
//  global.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "common.h"
#include "matrix4.h"
#include "array.h"
#include "memory.h"
#include "superobject.h"

struct FIX* fix = NULL;
struct LVL* lvl = NULL;
struct Engine* engine = NULL;

struct Camera *camera = NULL;

struct Actor* rayman = NULL;
struct Actor* camera_actor = NULL;
struct Actor* reflux = NULL;

int log_indent = 0;

int family_colors = 0;
uint32_t family_color_table[1000];
