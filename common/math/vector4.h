/*******************************************************
 * vector4.h: Four-component vector
 *******************************************************
 * Created by Jba03 on 2021-05-20
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef vector4_h
#define vector4_h

#include "mathc.h"
#include "macrotricks.h"

#define VECTOR4
VECTOR4 VECTOR_DEFINITION(float, 4)

/**
 * vector4_new:
 * Construct a new four-component vector
 */
VECTOR4 static inline struct vector4 vector4_new(const float x, const float y, const float z, const float w)
{
    struct vector4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    
    return v;
}

/**
 * vector4_add:
 * Add two four-component vectors
 */
VECTOR4 static inline struct vector4 vector4_add(const struct vector4 a, const struct vector4 b)
{
    struct vector4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    
    return result;
}

/**
 * vector4_sub:
 * Subtract four-component vectors
 */
VECTOR4 static inline struct vector4 vector4_sub(const struct vector4 a, const struct vector4 b)
{
    struct vector4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    
    return result;
}

/**
 * vector4_mul:
 * Multiply two four-component vectors
 */
VECTOR4 static inline struct vector4 vector4_mul(const struct vector4 a, const struct vector4 b)
{
    struct vector4 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    
    return result;
}

/**
 * vector4_div:
 * Divide four-component vector `a` by `b`
 */
VECTOR4 static inline struct vector4 vector4_div(const struct vector4 a, const struct vector4 b)
{
    struct vector4 result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    
    return result;
}

/**
 * vector4_mulf:
 * Multiply a four-component vector with a floating-point scalar value
 */
VECTOR4 static inline struct vector4 vector4_mulf(const struct vector4 vector, const float value)
{
    struct vector4 result;
    result.x = vector.x * value;
    result.y = vector.y * value;
    result.z = vector.z * value;
    result.w = vector.w * value;
    
    return result;
}


#endif /* vector4_h */
