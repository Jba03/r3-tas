/***************************************************
 * This file is not part of the structure library. *
 ***************************************************/

#include "stVector3D.h"

#include "mathc.h"

#include <assert.h>

#define box_face_none  0x00
#define box_face_x_min 0x01
#define box_face_x_max 0x02
#define box_face_y_min 0x04
#define box_face_y_max 0x08
#define box_face_z_min 0x10
#define box_face_z_max 0x20
#define box_face_all   0x3f

#define square(v) ((v) * (v))

static const float vectorindex(const tdstVector3D v, unsigned i)
{
    if (i == 0) return v.x;
    if (i == 1) return v.y;
    if (i == 2) return v.z;
    return 0.0f;
}

static const uint8 planecrossing(const tdstVector3D A, const tdstVector3D AB, const tdstVector3D bmin, const tdstVector3D bmax, float t, int8 face)
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

/** fnBoxPointIntersection: test if point lies in box */
static bool fnBoxPointIntersection(tdstVector3D bMin, tdstVector3D bMax, tdstVector3D p)
{
    return (p.x >= bMin.x && p.x <= bMax.x) && (p.y >= bMin.y && p.y <= bMax.y) && (p.z >= bMin.z && p.z <= bMax.z);
}

/** fnPointBoxFaceTest: check on which box face the point lies outside of */
static unsigned fnPointBoxFaceTest(tdstVector3D bmin, tdstVector3D bmax, tdstVector3D point)
{
    unsigned mask = box_face_none;
    point.x < bmin.x ? (mask |= box_face_x_min) : (point.x > bmax.x ? (mask |= box_face_x_max) : 0);
    point.y < bmin.y ? (mask |= box_face_y_min) : (point.y > bmax.y ? (mask |= box_face_y_max) : 0);
    point.z < bmin.z ? (mask |= box_face_z_min) : (point.z > bmax.z ? (mask |= box_face_z_max) : 0);
    return mask;
}

/** fnLineSegmentBoxIntersection: intersect box with line segment. AB is the offset from A. */
static bool fnLineSegmentBoxIntersection(tdstVector3D bmin, tdstVector3D bmax, tdstVector3D A, tdstVector3D AB, float *t)
{
    unsigned mA;
    unsigned mB;
    unsigned mXYZ;
    tdstVector3D B = vector3_add(A, AB);
    
    /* Test if any of the points are inside the box.  */
    mA = fnPointBoxFaceTest(bmin, bmax, A);
    mB = fnPointBoxFaceTest(bmin, bmax, B);
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

/** fnBoxBoxIntersection: box and box intersection test */
static bool fnBoxBoxIntersection(tdstVector3D aMin, tdstVector3D aMax, tdstVector3D bMin, tdstVector3D bMax)
{
    return max(aMin.x, bMin.x) <= min(aMax.x, bMax.x) &&
           max(aMin.y, bMin.y) <= min(aMax.y, bMax.y) &&
           max(aMin.z, bMin.z) <= min(aMax.z, bMax.z);
}

/** fnSphereBoxIntersection: sphere and box intersection test */
static bool fnSphereBoxIntersection(tdstVector3D sphCenter, float sphRadius, tdstVector3D bMin, tdstVector3D bMax)
{
    float d = 0.0f;
    
    if (sphCenter.x < bMin.x) d += square(sphCenter.x - bMin.x);
    else if (sphCenter.x > bMax.x) d += square(sphCenter.x - bMax.x);
    if (sphCenter.y < bMin.y) d += square(sphCenter.y - bMin.y);
    else if (sphCenter.y > bMax.y) d += square(sphCenter.y - bMax.y);
    if (sphCenter.z < bMin.z) d += square(sphCenter.z - bMin.z);
    else if (sphCenter.z > bMax.z) d += square(sphCenter.z - bMax.z);
    
    return d < sphRadius * sphRadius;
}

/** intersect_sphere_sphere: sphere and sphere intersection test */
/** fnSphereSphereIntersection: sphere and sphere intersection test */
static bool fnSphereSphereIntersection(tdstVector3D aCenter, float aRadius, tdstVector3D bCenter, float bRadius)
{
    return square(vector3_length(vector3_sub(aCenter, bCenter))) <= square(aRadius + bRadius);
}

#undef square
