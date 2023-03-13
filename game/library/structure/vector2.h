/*******************************************************
 * vector2.h: Two-component vector
 *******************************************************
 * Created by Jba03 on 2021-05-22
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef vector2_h
#define vector2_h

#include "mathc.h"

#define VECTOR2

struct vector2
{
    float x;
    float y;
};

VECTOR2 static inline struct vector2 vector2_host_byteorder(const struct vector2 v)
{
    struct vector2 result;
    result.x = host_byteorder_f32(*(uint32_t*)&v.x);
    result.y = host_byteorder_f32(*(uint32_t*)&v.y);
    
    return result;
}

/**
 * vector2_new:
 * Creates a new two-component vector
 */
VECTOR2 static inline struct vector2 vector2_new(const float x, const float y)
{
    struct vector2 v;
    v.x = x;
    v.y = y;
    
    return v;
}

/**
 * vector2_add:
 * Add two vectors
 */
VECTOR2 static inline struct vector2 vector2_add(const struct vector2 a, const struct vector2 b)
{
    struct vector2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    
    return result;
}

/**
 * vector2_sub:
 * Subtract vector `b` from vector `a`
 */
VECTOR2 static inline struct vector2 vector2_sub(const struct vector2 a, const struct vector2 b)
{
    struct vector2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    
    return result;
}

/**
 * vector2_mul:
 * Multiply two vectors
 */
VECTOR2 static inline struct vector2 vector2_mul(const struct vector2 a, const struct vector2 b)
{
    struct vector2 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    
    return result;
}

/**
 * vector2_mulf:
 * Multiply two vectors
 */
VECTOR2 static inline struct vector2 vector2_mulf(const struct vector2 a, const float value)
{
    struct vector2 result;
    result.x = a.x * value;
    result.y = a.y * value;
    
    return result;
}

/**
 * vector2_div:
 * Divide vector `a` by vector `b`
 */
VECTOR2 static inline struct vector2 vector2_div(const struct vector2 a, const struct vector2 b)
{
    struct vector2 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    
    return result;
}

/**
 * vector2_negate:
 * Negate a vector
 */
VECTOR2 static inline struct vector2 vector2_negate(const struct vector2 vector)
{
    struct vector2 result;
    result.x = -vector.x;
    result.y = -vector.y;
    
    return result;
}

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 static inline float vector2_dot(const struct vector2 a, const struct vector2 b)
{
    return (a.x * b.x) + (a.y * b.y);
}

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 static inline float vector2_length(const struct vector2 a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

#endif /* vector2_h */
