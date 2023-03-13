#ifndef intersect_h
#define intersect_h

#include "structure.h"
#include "vector3.h"
 
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
bool intersect_box_point(const struct vector3 bmin, const struct vector3 bmax, const struct vector3 point);

/** intersect_box_point_facetest: check on which box face the point lies outside of */
unsigned intersect_box_point_facetest(const struct vector3 bmin, const struct vector3 bmax, const struct vector3 point);

/** intersect_box_line_segment: intersect box with line segment. AB is the offset from A. */
bool intersect_box_line_segment(const struct vector3 bmin, const struct vector3 bmax, const struct vector3 A, const struct vector3 AB, float *t);

/** intersect_box_box: box and box intersection test */
bool intersect_box_box(const struct vector3 a_min, const struct vector3 a_max,
                       const struct vector3 b_min, const struct vector3 b_max);

/** intersect_sphere_box: sphere and box intersection test */
bool intersect_sphere_box(const struct vector3 sphere_center, const float radius,
                          const struct vector3 boxmin, const struct vector3 boxmax);

/** intersect_sphere_sphere: sphere and sphere intersection test */
bool intersect_sphere_sphere(const struct vector3 a_center, const float a_radius,
                             const struct vector3 b_center, const float b_radius);

#endif

#endif /* intersect_h */
