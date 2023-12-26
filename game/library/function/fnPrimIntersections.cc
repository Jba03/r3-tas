/***************************************************
 * This file is not part of the structure library. *
 ***************************************************/

#include "stVector3D.hh"
#include "stCollideElementIndexedTriangles.hh"

#include "mathc.hh"

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

static const float vectorindex(const stVector3D v, unsigned i)
{
    if (i == 0) return v.x;
    if (i == 1) return v.y;
    if (i == 2) return v.z;
    return 0.0f;
}

static const uint8 planecrossing(const stVector3D A, const stVector3D AB, const stVector3D bmin, const stVector3D bmax, float t, int8 face)
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
static bool fnBoxPointIntersection(stVector3D bMin, stVector3D bMax, stVector3D p)
{
    return (p.x >= bMin.x && p.x <= bMax.x) && (p.y >= bMin.y && p.y <= bMax.y) && (p.z >= bMin.z && p.z <= bMax.z);
}

/** fnPointBoxFaceTest: check on which box face the point lies outside of */
static unsigned fnPointBoxFaceTest(stVector3D bmin, stVector3D bmax, stVector3D point)
{
    unsigned mask = box_face_none;
    point.x < bmin.x ? (mask |= box_face_x_min) : (point.x > bmax.x ? (mask |= box_face_x_max) : 0);
    point.y < bmin.y ? (mask |= box_face_y_min) : (point.y > bmax.y ? (mask |= box_face_y_max) : 0);
    point.z < bmin.z ? (mask |= box_face_z_min) : (point.z > bmax.z ? (mask |= box_face_z_max) : 0);
    return mask;
}

/** fnLineSegmentBoxIntersection: intersect box with line segment. AB is the offset from A. */
static bool fnLineSegmentBoxIntersection(stVector3D bmin, stVector3D bmax, stVector3D A, stVector3D AB, float *t)
{
    unsigned mA;
    unsigned mB;
    unsigned mXYZ;
    stVector3D B = vector3_add(A, AB);
    
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
static bool fnBoxBoxIntersection(stVector3D aMin, stVector3D aMax, stVector3D bMin, stVector3D bMax)
{
    return max(aMin.x, bMin.x) <= min(aMax.x, bMax.x) &&
           max(aMin.y, bMin.y) <= min(aMax.y, bMax.y) &&
           max(aMin.z, bMin.z) <= min(aMax.z, bMax.z);
}

/** fnSphereBoxIntersection: sphere and box intersection test */
static bool fnSphereBoxIntersection(stVector3D sphCenter, float sphRadius, stVector3D bMin, stVector3D bMax)
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

/** fnSphereSphereIntersection: sphere and sphere intersection test */
static bool fnSphereSphereIntersection(stVector3D aCenter, float aRadius, stVector3D bCenter, float bRadius)
{
    return square(vector3_length(vector3_sub(aCenter, bCenter))) <= square(aRadius + bRadius);
}

static bool fnPointInSphere(const stVector3D P, const stVector3D C, const float r)
{
    return vector3_length(vector3_sub(P, C)) < r;
}

/** fnPointInTriangle: test if point lies in triangle **/
static bool fnPointInTriangle(const stVector3D P, const stVector3D A, const stVector3D B, const stVector3D C, const stVector3D N)
{
    const stVector3D p0 = vector3_sub(A, P);
    const stVector3D p1 = vector3_sub(B, P);
    const stVector3D p2 = vector3_sub(C, P);
    
    #define M_INV_SQRT_3 5.773502691896259E-1
    #define SameSign(a,b) ((a >= 0.0f) == (b >= 0.0f))
    
    #define MSM(a,b,c,d) ((a)*(b) - (c)*(d))
    #define dXY(a,b) MSM(a.x, b.y, a.y, b.x)
    #define dYZ(a,b) MSM(a.y, b.z, a.z, b.y)
    #define dZX(a,b) MSM(a.z, b.x, a.x, b.z)
    
    float det = 0.0f;
    if (fabs(N.z) >= M_INV_SQRT_3) {
        det = dXY(p0, p1);
        if (SameSign(det, N.z)) {
            det = dXY(p1, p2);
            if (SameSign(det, N.z)) {
                det = dXY(p2, p0);
                return SameSign(det, N.z);
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        if (fabs(N.x) >= M_INV_SQRT_3) {
            det = dYZ(p0, p1);
            if (SameSign(det, N.x)) {
                det = dYZ(p1, p2);
                if (SameSign(det, N.x)) {
                    det = dYZ(p2, p0);
                    return SameSign(det, N.x);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            det = dZX(p0, p1);
            if (SameSign(det, N.y)) {
                det = dZX(p1, p2);
                if (SameSign(det, N.y)) {
                    det = dZX(p2, p0);
                    return SameSign(det, N.y);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
}

static bool fnSegmentTriangleIntersection(const stVector3D start,
                                        const stVector3D extent,
                                        const stVector3D A,
                                        const stVector3D B,
                                        const stVector3D C,
                                        const stVector3D N,
                                        const float d)
{
    const float dot = vector3_dot(N, extent);
    
    if (dot < 0.0f)
    {
        float dividend = vector3_dot(N, start) + d;
        float t = -(dividend / dot);
        if (t >= 0.0f && t <= 1.0f)
        {
            const stVector3D P = vector3_add(vector3_mulf(extent, t), start);
            return fnPointInTriangle(P, A, B, C, N);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

#undef square
