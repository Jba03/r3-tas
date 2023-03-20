#ifndef intersect_h
#define intersect_h

#include "structure.h"
#include "stVector3D.h"
 
#define box_face_none  0x00
#define box_face_x_min 0x01
#define box_face_x_max 0x02
#define box_face_y_min 0x04
#define box_face_y_max 0x08
#define box_face_z_min 0x10
#define box_face_z_max 0x20
#define box_face_all   0x3f

#if USE_FUNCTIONS

/** intersect_box_point: test if point lies in box */
bool intersect_box_point(const tdstVector3D bmin, const tdstVector3D bmax, const tdstVector3D point);

/** intersect_box_point_facetest: check on which box face the point lies outside of */
unsigned intersect_box_point_facetest(const tdstVector3D bmin, const tdstVector3D bmax, const tdstVector3D point);

/** intersect_box_line_segment: intersect box with line segment. AB is the offset from A. */
bool intersect_box_line_segment(const tdstVector3D bmin, const tdstVector3D bmax, const tdstVector3D A, const tdstVector3D AB, float *t);

/** intersect_box_box: box and box intersection test */
bool intersect_box_box(const tdstVector3D a_min, const tdstVector3D a_max,
                       const tdstVector3D b_min, const tdstVector3D b_max);

/** intersect_sphere_box: sphere and box intersection test */
bool intersect_sphere_box(const tdstVector3D sphere_center, const float radius,
                          const tdstVector3D boxmin, const tdstVector3D boxmax);

/** intersect_sphere_sphere: sphere and sphere intersection test */
bool intersect_sphere_sphere(const tdstVector3D a_center, const float a_radius,
                             const tdstVector3D b_center, const float b_radius);

#endif

#endif /* intersect_h */
