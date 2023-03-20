//
//  graphics.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef graphics_h
#define graphics_h

//#include "vector3.h"
#include "stVector4D.h"
#include "mesh.h"

struct visualobject2
{
    struct vertex* vertices;
    unsigned int n_vertices;
    
    unsigned int* indices;
    unsigned int n_indices;
    
    tdstVector4D sphere_position;
};

void graphics_init(void);

void graphics_load(void);

void graphics_loop(void);

void graphics_unload(void);

void graphics_set_viewport(int width, int height);

void graphics_draw_box(tdstVector3D position, tdstVector3D scale, tdstVector3D rotation, tdstVector4D color);

void graphics_draw_line(tdstVector3D start, tdstVector3D end);

void graphics_draw_triangle(tdstVector3D a, tdstVector3D b, tdstVector3D c);

void graphics_draw_sphere(tdstVector3D center, const float radius, tdstVector4D color);

int graphics_shader_id(void);

void* graphics_get_texture(void);

void graphics_load_meshes(void);

#endif /* graphics_h */
