//
//  graphics.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef graphics_h
#define graphics_h

#include "vector3.h"
#include "vector4.h"

void graphics_init(void);

void graphics_draw_sphere(struct Vector3 center, const float radius, struct Vector4 color);

#endif /* graphics_h */
