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

#define RAY static inline

struct ray
{
    vector3 origin;
    vector3 direction;
};

RAY static inline struct ray raycast(const vector2 coord, const matrix4 projection, const matrix4 view, const float near, const float far)
{
    struct ray ray;
    
    const matrix4 combined = matrix4_mul(projection, view);
    const matrix4 inverse = matrix4_inverse(combined);

    const vector4 a = vector4_new(coord.x, coord.y, -1.0f, 1.0f);
    const vector4 p1 = vector4_mulf(a, near);
    const vector2 xy = vector2_mulf(coord, far - near);
    
    const vector4 p2 = vector4_new(xy.x, xy.y, far + near, far - near);
    const vector4 origin = vector4_mul_matrix4(p1, inverse);
    const vector4 direction = vector4_mul_matrix4(p2, inverse);
    
    ray.origin = vector3_new(origin.x, origin.y, origin.z);
    ray.direction = vector3_new(direction.x, direction.y, direction.z);
    
    return ray;
}

#endif /* ray_h */
