/*******************************************************
 * vector3.c: Three-component vector operations
 *******************************************************
 * Created by Jba03 on 2022-10-04
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#include "vector3.h"

#include <stdlib.h>
#include <math.h>

#pragma mark - Vector3

VECTOR3 struct Vector3 vector3_read(const address addr)
{
    struct Vector3 v = { .offset = addr };
    v.x = memory.read_float(addr + 4 * 0);
    v.y = memory.read_float(addr + 4 * 1);
    v.z = memory.read_float(addr + 4 * 2);
    
    return v;
}

VECTOR3 struct Vector3 vector3_new(const float x, const float y, const float z)
{
    struct Vector3 v;
    v.offset = 0x00;
    v.x = x;
    v.y = y;
    v.z = z;
    
    return v;
}


VECTOR3 struct Vector3 vector3_add(const struct Vector3 a, const struct Vector3 b)
{
    struct Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    
    return result;
}

VECTOR3 struct Vector3 vector3_sub(const struct Vector3 a, const struct Vector3 b)
{
    struct Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    
    return result;
}

VECTOR3 struct Vector3 vector3_mul(const struct Vector3 a, const struct Vector3 b)
{
    struct Vector3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    
    return result;
}

VECTOR3 struct Vector3 vector3_div(const struct Vector3 a, const struct Vector3 b)
{
    struct Vector3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    
    return result;
}

VECTOR3 struct Vector3 vector3_negate(const struct Vector3 vector)
{
    struct Vector3 result;
    result.x = -vector.x;
    result.y = -vector.y;
    result.z = -vector.z;
    
    return result;
}

VECTOR3 struct Vector3 vector3_mulf(const struct Vector3 vector, const float value)
{
    struct Vector3 result;
    result.x = vector.x * value;
    result.y = vector.y * value;
    result.z = vector.z * value;
    
    return result;
}

VECTOR3 const float vector3_dot(const struct Vector3 a, const struct Vector3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

VECTOR3 struct Vector3 vector3_cross(const struct Vector3 a, const struct Vector3 b)
{
    struct Vector3 result;
    result.x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
    result.y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
    result.z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
    
    return result;
}

VECTOR3 struct Vector3 vector3_wedge(const struct Vector3 a, const struct Vector3 b)
{
    struct Vector3 result;
    result.x = (a.y * b.z) - (b.y * a.z);
    result.y = (a.z * b.x) - (b.z * a.x);
    result.z = (a.x * b.y) - (b.x * a.y);
    
    return result;
}

VECTOR3 const float vector3_length(const struct Vector3 vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

VECTOR3 struct Vector3 vector3_normalize(const struct Vector3 vector)
{
    const float length = vector3_length(vector);
    const float scale = 1.0f / length;
    
    struct Vector3 result;
    result.x = vector.x * scale;
    result.y = vector.y * scale;
    result.z = vector.z * scale;
    
    return length == 0.0f ? vector : result;
}

VECTOR3 struct Vector3 vector3_random()
{
    struct Vector3 result;
    result.x = drand48();
    result.y = drand48();
    result.z = drand48();
    
    return result;
}
