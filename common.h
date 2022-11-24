//
//  global.h
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef global_h
#define global_h

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "stream.h"
#include "logging.h"

#define min(a, b) (((a) < (b)) ? (a) : (b)) /* min: Choose smaller of two values */
#define max(a, b) (((a) > (b)) ? (a) : (b)) /* max: Choose bigger of two values */

#define radians(deg) ((deg) * (M_PI / 180.0f))
#define degrees(rad) ((rad) * (180.0f / M_PI))

extern struct FIX* fix;
extern struct LVL* lvl;
extern struct Engine* engine;

extern int log_indent;

extern int family_colors;
extern uint32_t family_color_table[1000];

#endif /* global_h */
