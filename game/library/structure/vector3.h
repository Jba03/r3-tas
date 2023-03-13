/*******************************************************
 * vector3.h: Three-component vector
 *******************************************************
 * Created by Jba03 on 2021-05-20
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef vector3_h
#define vector3_h

#include <stdlib.h>

#include "mathc.h"
#include "structure.h"

#define VECTOR3

struct vector3
{
    float x;
    float y;
    float z;
};

VECTOR3 static inline struct vector3 vector3_host_byteorder(const struct vector3 v)
{
    struct vector3 result;
    result.x = host_byteorder_f32(*(uint32_t*)&v.x);
    result.y = host_byteorder_f32(*(uint32_t*)&v.y);
    result.z = host_byteorder_f32(*(uint32_t*)&v.z);
    
    return result;
}

/**
 * struct vector3_new:
 * Construct a new three-component vector
 */
VECTOR3 static inline struct vector3 vector3_new(const float x, const float y, const float z)
{
    struct vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    
    return v;
}

/**
 * struct vector3_add:
 * Add two vectors
 */
VECTOR3 static inline struct vector3 vector3_add(const struct vector3 a, const struct vector3 b)
{
    struct vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    
    return result;
}

/**
 * struct vector3_sub:
 * Subtracts vector `b` from vector `a`
 */
VECTOR3 static inline struct vector3 vector3_sub(const struct vector3 a, const struct vector3 b)
{
    struct vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    
    return result;
}

/**
 * struct vector3_mul:
 * Multiplies vector `a` with vector `b`
 */
VECTOR3 static inline struct vector3 vector3_mul(const struct vector3 a, const struct vector3 b)
{
    struct vector3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    
    return result;
}

/**
 * struct vector3_div:
 * Divides vector `a` with vector `b`
 */
VECTOR3 static inline struct vector3 vector3_div(const struct vector3 a, const struct vector3 b)
{
    struct vector3 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    
    return result;
}

/**
 * struct vector3_negate:
 * Negate a vector
 */
VECTOR3 static inline struct vector3 vector3_negate(const struct vector3 vector)
{
    struct vector3 result;
    result.x = -vector.x;
    result.y = -vector.y;
    result.z = -vector.z;
    
    return result;
}

/**
 * struct vector3_mulf:
 * Multiply a vector by a scalar float value
 */
VECTOR3 static inline struct vector3 vector3_mulf(const struct vector3 vector, const float value)
{
    struct vector3 result;
    result.x = vector.x * value;
    result.y = vector.y * value;
    result.z = vector.z * value;
    
    return result;
}

/**
 * struct vector3_dot:
 * Computes the dot product a•b of two vectors `a` and `b`
 */
VECTOR3 static inline const float vector3_dot(const struct vector3 a, const struct vector3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/**
 * struct vector3_cross:
 * Calculate the vector cross product. The cross product of two vectors
 * is a vector that is perpendicular (therefore normal) to the two vectors.
 */
VECTOR3 static inline struct vector3 vector3_cross(const struct vector3 a, const struct vector3 b)
{
    struct vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    
    return result;
}

/**
 * struct vector3_wedge:
 * I have no idea what this does.
 */
VECTOR3 static inline struct vector3 vector3_wedge(const struct vector3 a, const struct vector3 b)
{
    struct vector3 result;
    result.x = (a.y * b.z) - (b.y * a.z);
    result.y = (a.z * b.x) - (b.z * a.x);
    result.z = (a.x * b.y) - (b.x * a.y);
    
    return result;
}

/**
 * struct vector3_length:
 * Computes the length of a vector, i.e. the square root of
 * the total sum of the vector's squared components.
 */
VECTOR3 static inline const float vector3_length(const struct vector3 vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

/**
 * struct vector3_normalize:
 * Normalizes a 3-component vector. In case the input
 * vector has a length of zero, the input vector is returned.
 *
 * v = v * 1 / |v|
 */
VECTOR3 static inline struct vector3 vector3_normalize(const struct vector3 vector)
{
    const float length = vector3_length(vector);
    const float scale = 1.0f / length;
    
    struct vector3 result;
    result.x = vector.x * scale;
    result.y = vector.y * scale;
    result.z = vector.z * scale;
    
    return length == 0.0f ? vector : result;
}

/**
 * struct vector3_random:
 * Returns a random 3-component unit vector
 */
VECTOR3 static inline struct vector3 vector3_random()
{
    struct vector3 result;
    result.x = (float)rand() / (float)RAND_MAX;
    result.y = (float)rand() / (float)RAND_MAX;
    result.z = (float)rand() / (float)RAND_MAX;
    
    return result;
}

#endif /* vector3_h */
