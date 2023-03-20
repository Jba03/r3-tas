#ifndef stVector2D_h
#define stVector2D_h

#include "mathc.h"

#define VECTOR2

struct stVector2D {
    float x;
    float y;
} typedef tdstVector2D;

VECTOR2 static inline tdstVector2D vector2_host_byteorder(const tdstVector2D v)
{
    tdstVector2D result;
    result.x = host_byteorder_f32(*(uint32_t*)&v.x);
    result.y = host_byteorder_f32(*(uint32_t*)&v.y);
    
    return result;
}

/**
 * vector2_new:
 * Creates a new two-component vector
 */
VECTOR2 static inline tdstVector2D vector2_new(const float x, const float y)
{
    tdstVector2D v;
    v.x = x;
    v.y = y;
    
    return v;
}

/**
 * vector2_add:
 * Add two vectors
 */
VECTOR2 static inline tdstVector2D vector2_add(const tdstVector2D a, const tdstVector2D b)
{
    tdstVector2D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    
    return result;
}

/**
 * vector2_sub:
 * Subtract vector `b` from vector `a`
 */
VECTOR2 static inline tdstVector2D vector2_sub(const tdstVector2D a, const tdstVector2D b)
{
    tdstVector2D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    
    return result;
}

/**
 * vector2_mul:
 * Multiply two vectors
 */
VECTOR2 static inline tdstVector2D vector2_mul(const tdstVector2D a, const tdstVector2D b)
{
    tdstVector2D result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    
    return result;
}

/**
 * vector2_mulf:
 * Multiply two vectors
 */
VECTOR2 static inline tdstVector2D vector2_mulf(const tdstVector2D a, const float value)
{
    tdstVector2D result;
    result.x = a.x * value;
    result.y = a.y * value;
    
    return result;
}

/**
 * vector2_div:
 * Divide vector `a` by vector `b`
 */
VECTOR2 static inline tdstVector2D vector2_div(const tdstVector2D a, const tdstVector2D b)
{
    tdstVector2D result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    
    return result;
}

/**
 * vector2_negate:
 * Negate a vector
 */
VECTOR2 static inline tdstVector2D vector2_negate(const tdstVector2D vector)
{
    tdstVector2D result;
    result.x = -vector.x;
    result.y = -vector.y;
    
    return result;
}

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 static inline float vector2_dot(const tdstVector2D a, const tdstVector2D b)
{
    return (a.x * b.x) + (a.y * b.y);
}

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 static inline float vector2_length(const tdstVector2D a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

#endif /* stVector2D_h */
