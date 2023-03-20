/*******************************************************
 * ray.h: Raycasting functions
 *******************************************************
 * Created by Jba03 on 2022-11-27
 * Copyright (c) 2022 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef ray_h
#define ray_h

#include "stVector2D.h"
#include "stVector3D.h"
#include "stMatrix4D.h"

#define RAY

struct ray
{
    tdstVector3D origin;
    tdstVector3D direction;
};

RAY static inline struct ray raycast(const tdstVector2D coord, const tdstMatrix4D projection, const tdstMatrix4D view, const float near, const float far)
{
    struct ray ray;
    
    const tdstMatrix4D combined = matrix4_mul(projection, view);
    const tdstMatrix4D inverse = matrix4_inverse(combined);

    const tdstVector4D a = vector4_new(coord.x, coord.y, -1.0f, 1.0f);
    const tdstVector4D p1 = vector4_mulf(a, near);
    const tdstVector2D xy = vector2_mulf(coord, far - near);
    
    const tdstVector4D p2 = vector4_new(xy.x, xy.y, far + near, far - near);
    const tdstVector4D origin = vector4_mul_matrix4(p1, inverse);
    const tdstVector4D direction = vector4_mul_matrix4(p2, inverse);
    
    ray.origin = vector3_new(origin.x, origin.y, origin.z);
    ray.direction = vector3_new(direction.x, direction.y, direction.z);
    
    return ray;
}

RAY static inline bool ray_triangle_intersection(const struct ray ray, const tdstVector3D A, const tdstVector3D B, const tdstVector3D C, float* t)
{
    #define EPSILON 1e-6
    #define CULLING 1
    
    tdstVector3D E1 = vector3_sub(B, A);
    tdstVector3D E2 = vector3_sub(C, A);
    tdstVector3D H = vector3_cross(ray.direction, E2);
    
    const double det = vector3_dot(E1, H);
    const double invdet = 1.0f / det;
#if CULLING
    if (det < EPSILON) return false;
#else
    if (fabs(det) < EPSILON) return false;
#endif
    
    const tdstVector3D S = vector3_sub(ray.origin, A);
    const double u = vector3_dot(S, H) * invdet;
    if (u < 0.0f || u > 1.0f) return false;
    
    const tdstVector3D Q = vector3_cross(S, E1);
    const double v = vector3_dot(ray.direction, Q) * invdet;
    if (v < 0.0f || u + v > 1.0f) return false;
    
    *t = invdet * vector3_dot(E2, Q);
    return *t > EPSILON;
}

#endif /* ray_h */
