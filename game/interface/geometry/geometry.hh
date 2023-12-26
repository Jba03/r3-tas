//
//  geometry.h
//  r3-tas
//
//  Created by Jba03 on 2023-02-23.
//

#ifndef geometry_h
#define geometry_h

#include "game.hh"
#include "stInstantiatedPhysicalObject.hh"
#include "stVector3D.hh"
#include "stCollideObject.hh"
#include "stCollideElementIndexedTriangles.hh"

struct triangle
{
    stVector3D a;
    stVector3D b;
    stVector3D c;
};

/**
 * collide_object_triangles_combined:
 *  Get a list of all combined triangles in a collide object.
 *  Returns the number of triangles in the list.
 */
int collide_object_triangles_combined(const stCollideObject* object, const stMatrix4D transform, struct triangle** triangles);


/**
 * collide_object_closest_vertex_to:
 * In a specified collide mesh, find the vertex which lies the closest to a certain point.
 */
int collide_object_closest_vertex_to(const stCollideObject* object,
                                     const stMatrix4D object_transform,
                                     const stVector3D point,
                                     stCollideElementIndexedTriangles** mesh_out,
                                     int16 *index_out,
                                     stVector3D *vertex_out);



const stVector3D ipo_closest_vertex_to_point();

#endif /* geometry_h */
