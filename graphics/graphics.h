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

void graphics_set_viewport(int width, int height);

void graphics_draw_box(struct Vector3 position, struct Vector3 scale, struct Vector3 rotation, struct Vector4 color);

void graphics_draw_line(struct Vector3 start, struct Vector3 end);

void graphics_draw_triangle(struct Vector3 a, struct Vector3 b, struct Vector3 c);

void graphics_draw_sphere(struct Vector3 center, const float radius, struct Vector4 color);

int graphics_shader_id(void);

#endif /* graphics_h */
