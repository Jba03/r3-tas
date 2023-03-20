#ifndef stVector3D_h
#define stVector3D_h

#include "structure.h"

#include <math.h>
#include <stdlib.h>

#define VECTOR3

struct stVector3D {
    float x;
    float y;
    float z;
} typedef tdstVector3D;

VECTOR3 static inline tdstVector3D vector3_host_byteorder(const tdstVector3D v)
{
    tdstVector3D result;
    result.x = host_byteorder_f32(*(uint32_t*)&v.x);
    result.y = host_byteorder_f32(*(uint32_t*)&v.y);
    result.z = host_byteorder_f32(*(uint32_t*)&v.z);
    
    return result;
}

/**
 * tdstVector3D_new:
 * Construct a new three-component vector
 */
VECTOR3 static inline tdstVector3D vector3_new(const float x, const float y, const float z)
{
    tdstVector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    
    return v;
}

/**
 * tdstVector3D_add:
 * Add two vectors
 */
VECTOR3 static inline tdstVector3D vector3_add(const tdstVector3D a, const tdstVector3D b)
{
    tdstVector3D result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    
    return result;
}

/**
 * tdstVector3D_sub:
 * Subtracts vector `b` from vector `a`
 */
VECTOR3 static inline tdstVector3D vector3_sub(const tdstVector3D a, const tdstVector3D b)
{
    tdstVector3D result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    
    return result;
}

/**
 * tdstVector3D_mul:
 * Multiplies vector `a` with vector `b`
 */
VECTOR3 static inline tdstVector3D vector3_mul(const tdstVector3D a, const tdstVector3D b)
{
    tdstVector3D result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    
    return result;
}

/**
 * tdstVector3D_div:
 * Divides vector `a` with vector `b`
 */
VECTOR3 static inline tdstVector3D vector3_div(const tdstVector3D a, const tdstVector3D b)
{
    tdstVector3D result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    
    return result;
}

/**
 * tdstVector3D_negate:
 * Negate a vector
 */
VECTOR3 static inline tdstVector3D vector3_negate(const tdstVector3D vector)
{
    tdstVector3D result;
    result.x = -vector.x;
    result.y = -vector.y;
    result.z = -vector.z;
    
    return result;
}

/**
 * tdstVector3D_mulf:
 * Multiply a vector by a scalar float value
 */
VECTOR3 static inline tdstVector3D vector3_mulf(const tdstVector3D vector, const float value)
{
    tdstVector3D result;
    result.x = vector.x * value;
    result.y = vector.y * value;
    result.z = vector.z * value;
    
    return result;
}

/**
 * tdstVector3D_dot:
 * Computes the dot product a•b of two vectors `a` and `b`
 */
VECTOR3 static inline const float vector3_dot(const tdstVector3D a, const tdstVector3D b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/**
 * tdstVector3D_cross:
 * Calculate the vector cross product. The cross product of two vectors
 * is a vector that is perpendicular (therefore normal) to the two vectors.
 */
VECTOR3 static inline tdstVector3D vector3_cross(const tdstVector3D a, const tdstVector3D b)
{
    tdstVector3D result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    
    return result;
}

/**
 * tdstVector3D_wedge:
 * I have no idea what this does.
 */
VECTOR3 static inline tdstVector3D vector3_wedge(const tdstVector3D a, const tdstVector3D b)
{
    tdstVector3D result;
    result.x = (a.y * b.z) - (b.y * a.z);
    result.y = (a.z * b.x) - (b.z * a.x);
    result.z = (a.x * b.y) - (b.x * a.y);
    
    return result;
}

/**
 * tdstVector3D_length:
 * Computes the length of a vector, i.e. the square root of
 * the total sum of the vector's squared components.
 */
VECTOR3 static inline const float vector3_length(const tdstVector3D vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

/**
 * tdstVector3D_normalize:
 * Normalizes a 3-component vector. In case the input
 * vector has a length of zero, the input vector is returned.
 *
 * v = v * 1 / |v|
 */
VECTOR3 static inline tdstVector3D vector3_normalize(const tdstVector3D vector)
{
    const float length = vector3_length(vector);
    const float scale = 1.0f / length;
    
    tdstVector3D result;
    result.x = vector.x * scale;
    result.y = vector.y * scale;
    result.z = vector.z * scale;
    
    return length == 0.0f ? vector : result;
}

/**
 * tdstVector3D_random:
 * Returns a random 3-component unit vector
 */
VECTOR3 static inline tdstVector3D vector3_random()
{
    tdstVector3D result;
    result.x = (float)rand() / (float)RAND_MAX;
    result.y = (float)rand() / (float)RAND_MAX;
    result.z = (float)rand() / (float)RAND_MAX;
    
    return result;
}

#endif /* stVector3D_h */
