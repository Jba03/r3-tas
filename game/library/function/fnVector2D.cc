//
//  fnVector2D.c
//  r3-tas
//
//  Created by Jba03 on 2023-04-17.
//

#include "stVector2D.hh"
#include "memory.hh"

VECTOR2 stVector2D vector2_host_byteorder(const stVector2D v)
{
    stVector2D result;
    result.x = host_byteorder_f32(*(uint32_t*)&v.x);
    result.y = host_byteorder_f32(*(uint32_t*)&v.y);
    
    return result;
}

/**
 * vector2_new:
 * Creates a new two-component vector
 */
VECTOR2 stVector2D vector2_new(const float x, const float y)
{
    stVector2D v;
    v.x = x;
    v.y = y;
    
    return v;
}

/**
 * vector2_add:
 * Add two vectors
 */
VECTOR2 stVector2D vector2_add(const stVector2D a, const stVector2D b)
{
    stVector2D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    
    return result;
}

/**
 * vector2_sub:
 * Subtract vector `b` from vector `a`
 */
VECTOR2 stVector2D vector2_sub(const stVector2D a, const stVector2D b)
{
    stVector2D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    
    return result;
}

/**
 * vector2_mul:
 * Multiply two vectors
 */
VECTOR2 stVector2D vector2_mul(const stVector2D a, const stVector2D b)
{
    stVector2D result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    
    return result;
}

/**
 * vector2_mulf:
 * Multiply two vectors
 */
VECTOR2 stVector2D vector2_mulf(const stVector2D a, const float value)
{
    stVector2D result;
    result.x = a.x * value;
    result.y = a.y * value;
    
    return result;
}

/**
 * vector2_div:
 * Divide vector `a` by vector `b`
 */
VECTOR2 stVector2D vector2_div(const stVector2D a, const stVector2D b)
{
    stVector2D result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    
    return result;
}

/**
 * vector2_negate:
 * Negate a vector
 */
VECTOR2 stVector2D vector2_negate(const stVector2D vector)
{
    stVector2D result;
    result.x = -vector.x;
    result.y = -vector.y;
    
    return result;
}

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 float vector2_dot(const stVector2D a, const stVector2D b)
{
    return (a.x * b.x) + (a.y * b.y);
}

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 float vector2_length(const stVector2D a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}
