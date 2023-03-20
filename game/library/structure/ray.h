/*******************************************************
 * ray.h: Raycasting functions
 *******************************************************
 * Created by Jba03 on 2022-11-27
 * Copyright (c) 2022 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef ray_h
#define ray_h

#include "vector2.h"
#include "vector3.h"
#include "matrix4.h"

#define RAY

struct ray
{
    struct vector3 origin;
    struct vector3 direction;
};

RAY static inline struct ray raycast(const struct vector2 coord, const struct matrix4 projection, const struct matrix4 view, const float near, const float far)
{
    struct ray ray;
    
    const matrix4 combined = matrix4_mul(projection, view);
    const matrix4 inverse = matrix4_inverse(combined);

    const struct vector4 a = vector4_new(coord.x, coord.y, -1.0f, 1.0f);
    const struct vector4 p1 = vector4_mulf(a, near);
    const struct vector2 xy = vector2_mulf(coord, far - near);
    
    const struct vector4 p2 = vector4_new(xy.x, xy.y, far + near, far - near);
    const struct vector4 origin = vector4_mul_matrix4(p1, inverse);
    const struct vector4 direction = vector4_mul_matrix4(p2, inverse);
    
    ray.origin = vector3_new(origin.x, origin.y, origin.z);
    ray.direction = vector3_new(direction.x, direction.y, direction.z);
    
    return ray;
}

RAY static inline bool ray_triangle_intersection(const struct ray ray, const struct vector3 A, const struct vector3 B, const struct vector3 C, float* t)
{
    #define EPSILON 1e-6
    #define CULLING 1
    
    struct vector3 E1 = vector3_sub(B, A);
    struct vector3 E2 = vector3_sub(C, A);
    struct vector3 H = vector3_cross(ray.direction, E2);
    
    const double det = vector3_dot(E1, H);
    const double invdet = 1.0f / det;
#if CULLING
    if (det < EPSILON) return false;
#else
    if (fabs(det) < EPSILON) return false;
#endif
    
    const struct vector3 S = vector3_sub(ray.origin, A);
    const double u = vector3_dot(S, H) * invdet;
    if (u < 0.0f || u > 1.0f) return false;
    
    const struct vector3 Q = vector3_cross(S, E1);
    const double v = vector3_dot(ray.direction, Q) * invdet;
    if (v < 0.0f || u + v > 1.0f) return false;
    
    *t = invdet * vector3_dot(E2, Q);
    return *t > EPSILON;
}

#endif /* ray_h */
