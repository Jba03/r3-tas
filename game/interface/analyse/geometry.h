//
//  geometry.h
//  r3-tas
//
//  Created by Jba03 on 2023-02-23.
//

#ifndef geometry_h
#define geometry_h

#include "game.h"
#include "ipo.h"
#include "vector3.h"
#include "collide_object.h"
#include "collide_mesh.h"

struct triangle
{
    struct vector3 a;
    struct vector3 b;
    struct vector3 c;
};

/**
 * collide_object_triangles_combined:
 *  Get a list of all combined triangles in a collide object.
 *  Returns the number of triangles in the list.
 */
int collide_object_triangles_combined(const struct collide_object* object, const struct matrix4 transform, struct triangle** triangles);


/**
 * collide_object_closest_vertex_to:
 * In a specified collide mesh, find the vertex which lies the closest to a certain point.
 */
int collide_object_closest_vertex_to(const struct collide_object* object,
                                     const struct matrix4 object_transform,
                                     const struct vector3 point,
                                     struct collide_mesh** mesh_out,
                                     int16 *index_out,
                                     struct vector3 *vertex_out);



const struct vector3 ipo_closest_vertex_to_point();

#endif /* geometry_h */
