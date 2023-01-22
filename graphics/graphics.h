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
#include "mesh.h"

struct visualobject
{
    struct vertex* vertices;
    unsigned int n_vertices;
    
    unsigned int* indices;
    unsigned int n_indices;
    
    struct vector4 sphere_position;
};

void graphics_init(void);

void graphics_load(void);

void graphics_set_viewport(int width, int height);

void graphics_draw_box(struct vector3 position, struct vector3 scale, struct vector3 rotation, struct vector4 color);

void graphics_draw_line(struct vector3 start, struct vector3 end);

void graphics_draw_triangle(struct vector3 a, struct vector3 b, struct vector3 c);

void graphics_draw_sphere(struct vector3 center, const float radius, struct vector4 color);

int graphics_shader_id(void);

int graphics_get_texture(void);

void graphics_load_meshes(void);

#endif /* graphics_h */
