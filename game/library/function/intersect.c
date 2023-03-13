//
//  intersect.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include "intersect.h"
#include "mathc.h"

#include <assert.h>

#define square(v) ((v) * (v))

static const float vectorindex(const struct vector3 v, unsigned i)
{
    if (i == 0) return v.x;
    if (i == 1) return v.y;
    if (i == 2) return v.z;
    return 0.0f;
}

static const uint8 planecrossing(const struct vector3 A, const struct vector3 AB, const struct vector3 bmin, const struct vector3 bmax, float t, int8 face)
{
    unsigned i1 = 0;
    unsigned i2 = 0;

    if (face == 'x') { i1 = 1; i2 = 2; }
    if (face == 'y') { i1 = 0; i2 = 2; }
    if (face == 'z') { i1 = 0; i2 = 1; }
    
    float v = vectorindex(AB, i1) * t + vectorindex(A, i1);
    if (vectorindex(AB, i1) > 0.0f)
    {
        if (v > vectorindex(bmax, i1)) return 0;
        if (v >= vectorindex(bmin, i1))
        {
            v = vectorindex(AB, i2) * t + vectorindex(A, i2);
            if (vectorindex(AB, i2) > 0.0f)
            {
                if (v > vectorindex(bmax, i2)) return 0;
                if (v >= vectorindex(bmin, i2)) return 1;
                /* ? */
            }
            else
            {
                if (v < vectorindex(bmin, i2)) return 0;
                if (v <= vectorindex(bmax, i2)) return 1;
                /* ? */
            }
        }
        /* ? */
    }
    else
    {
        if (v > vectorindex(bmin, i1)) return 0;
        if (v >= vectorindex(bmax, i1))
        {
            v = vectorindex(AB, i2) * t + vectorindex(A, i2);
            if (vectorindex(AB, i2) > 0.0f)
            {
                if (v > vectorindex(bmax, i2)) return 0;
                if (v >= vectorindex(bmin, i2)) return 1;
                /* ? */
            }
            else
            {
                if (v < vectorindex(bmin, i2)) return 0;
                if (v <= vectorindex(bmax, i2)) return 1;
                /* ? */
            }
        }
        /* ? */
    }
    
    return 2;
}

/** intersect_box_point: test if point lies in box */
bool intersect_box_point(const struct vector3 bmin, const struct vector3 bmax, const struct vector3 p)
{
    return (p.x >= bmin.x && p.x <= bmax.x) && (p.y >= bmin.y && p.y <= bmax.y) && (p.z >= bmin.z && p.z <= bmax.z);
}

/** intersect_box_point_facetest: check on which box face the point lies outside of */
unsigned intersect_box_point_facetest(const struct vector3 bmin, const struct vector3 bmax, const struct vector3 point)
{
    unsigned mask = box_face_none;
    point.x < bmin.x ? (mask |= box_face_x_min) : (point.x > bmax.x ? (mask |= box_face_x_max) : 0);
    point.y < bmin.y ? (mask |= box_face_y_min) : (point.y > bmax.y ? (mask |= box_face_y_max) : 0);
    point.z < bmin.z ? (mask |= box_face_z_min) : (point.z > bmax.z ? (mask |= box_face_z_max) : 0);
    return mask;
}

/** intersect_box_line_segment: intersect box with line segment. AB is the offset from A. */
bool intersect_box_line_segment(const struct vector3 bmin, const struct vector3 bmax, const struct vector3 A, const struct vector3 AB, float *t)
{
    unsigned mA;
    unsigned mB;
    unsigned mXYZ;
    struct vector3 B = vector3_add(A, AB);
    
    /* Test if any of the points are inside the box.  */
    mA = intersect_box_point_facetest(bmin, bmax, A);
    mB = intersect_box_point_facetest(bmin, bmax, B);
    /* The points are inside only if they are outside none of the planes. */
    if (mA == box_face_none) { if (t) *t = 0.0f; return true; }
    if (mB == box_face_none) { if (t) *t = 1.0f; return true; }
    
    /* Both points are outside of the box. If the masks match,
     * they are outside the same face, so intersection is impossible. */
    if (mA & mB) return false;

    mXYZ = mA | mB;
    
    #define T(f1,f2,f3,f4,c,XYZ) if (!(mXYZ & (f1 | f2 | f3 | f4))) { if (!t) return true; \
    if (mA & box_face_##XYZ##_min && t) *t = bmin.XYZ - A.XYZ; else if (t) *t = bmax.XYZ - A.XYZ; \
    if (*t) *t /= AB.XYZ; return true; }
    T(box_face_x_min, box_face_x_max, box_face_y_min, box_face_y_max, box_face_z_min, z);
    T(box_face_x_min, box_face_x_max, box_face_z_min, box_face_z_max, box_face_y_min, y);
    T(box_face_z_min, box_face_z_max, box_face_y_min, box_face_y_max, box_face_x_min, x);
    #undef T
    
    mXYZ = mA;
    
    #define T(XYZ,cXYZ,minmax) if (mXYZ & box_face_##XYZ##_##minmax) { float t2 = b##minmax.XYZ - A.XYZ; \
    /*assert(AB.XYZ > 0.0f); assert(t2 > 0.0f);*/ t2 /= AB.XYZ; uint8 v; if (t) *t = t2; \
    if ((v = planecrossing(A, AB, bmin, bmax, t2, cXYZ)) < 2) return v; }
    T(x, 'x', min)
    T(x, 'x', max)
    T(y, 'y', min)
    T(y, 'y', max)
    T(z, 'z', min)
    T(z, 'z', max)
    #undef T
    
    return false;
}

/** intersect_box_box: box and box intersection test */
bool intersect_box_box(const struct vector3 a_min, const struct vector3 a_max,
                       const struct vector3 b_min, const struct vector3 b_max)
{
    return max(a_min.x, b_min.x) <= min(a_max.x, b_max.x) &&
           max(a_min.y, b_min.y) <= min(a_max.y, b_max.y) &&
           max(a_min.z, b_min.z) <= min(a_max.z, b_max.z);
}

/** intersect_sphere_box: sphere and box intersection test */
bool intersect_sphere_box(const struct vector3 c, const float r,
                          const struct vector3 bmin, const struct vector3 bmax)
{
    float d = 0.0f;
    
    if (c.x < bmin.x) d += square(c.x - bmin.x);
    else if (c.x > bmax.x) d += square(c.x - bmax.x);
    if (c.y < bmin.y) d += square(c.y - bmin.y);
    else if (c.y > bmax.y) d += square(c.y - bmax.y);
    if (c.z < bmin.z) d += square(c.z - bmin.z);
    else if (c.z > bmax.z) d += square(c.z - bmax.z);
    
    return d < r * r;
}

/** intersect_sphere_sphere: sphere and sphere intersection test */
bool intersect_sphere_sphere(const struct vector3 ca, const float ra,
                             const struct vector3 cb, const float rb)
{
    return square(vector3_length(vector3_sub(ca, cb))) <= square(ra + rb);
}

#undef square
