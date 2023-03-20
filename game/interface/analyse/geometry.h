//
//  geometry.h
//  r3-tas
//
//  Created by Jba03 on 2023-02-23.
//

#ifndef geometry_h
#define geometry_h

#include "game.h"
#include "stInstantiatedPhysicalObject.h"
#include "stVector3D.h"
#include "stCollideObject.h"
#include "stCollideElementIndexedTriangles.h"

struct triangle
{
    tdstVector3D a;
    tdstVector3D b;
    tdstVector3D c;
};

/**
 * collide_object_triangles_combined:
 *  Get a list of all combined triangles in a collide object.
 *  Returns the number of triangles in the list.
 */
int collide_object_triangles_combined(const tdstCollideObject* object, const tdstMatrix4D transform, struct triangle** triangles);


/**
 * collide_object_closest_vertex_to:
 * In a specified collide mesh, find the vertex which lies the closest to a certain point.
 */
int collide_object_closest_vertex_to(const tdstCollideObject* object,
                                     const tdstMatrix4D object_transform,
                                     const tdstVector3D point,
                                     tdstCollideElementIndexedTriangles** mesh_out,
                                     int16 *index_out,
                                     tdstVector3D *vertex_out);



const tdstVector3D ipo_closest_vertex_to_point();

#endif /* geometry_h */
