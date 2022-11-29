//
//  ray.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#include "ray.h"

RAY const struct Ray raycast(const struct Vector2 coord,
                             const struct Matrix4 projection,
                             const struct Matrix4 view,
                             const float near,
                             const float far)
{
    struct Ray ray;
    
    const struct Matrix4 combined = matrix4_mul(projection, view);
    const struct Matrix4 inverse = matrix4_inverse(combined);

    const struct Vector4 a = vector4_new(coord.x, coord.y, -1.0f, 1.0f);
    const struct Vector4 p1 = vector4_mulf(a, near);
    const struct Vector2 xy = vector2_mulf(coord, far - near);
    
    const struct Vector4 p2 = vector4_new(xy.x, xy.y, far + near, far - near);
    const struct Vector4 origin = vector4_mul_matrix4(p1, inverse);
    const struct Vector4 direction = vector4_mul_matrix4(p2, inverse);
    
    ray.origin = vector3_new(origin.x, origin.y, origin.z);
    ray.direction = vector3_new(direction.x, direction.y, direction.z);
    
    return ray;
}
