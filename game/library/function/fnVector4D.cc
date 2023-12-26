//
//  fnVector4D.c
//  r3-tas
//
//  Created by Jba03 on 2023-04-17.
//

#include "stVector4D.hh"
#include "memory.hh"

VECTOR4 tdstVector4D vector4_host_byteorder(const tdstVector4D v)
{
    tdstVector4D result;
    result.x = host_byteorder_f32(*(uint32_t*)&v.x);
    result.y = host_byteorder_f32(*(uint32_t*)&v.y);
    result.z = host_byteorder_f32(*(uint32_t*)&v.z);
    result.w = host_byteorder_f32(*(uint32_t*)&v.w);
    
    return result;
}

/**
 * vector4_new:
 * Construct a new four-component vector
 */
VECTOR4 tdstVector4D vector4_new(const float x, const float y, const float z, const float w)
{
    tdstVector4D v;
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
VECTOR4 tdstVector4D vector4_add(const tdstVector4D a, const tdstVector4D b)
{
    tdstVector4D result;
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
VECTOR4 tdstVector4D vector4_sub(const tdstVector4D a, const tdstVector4D b)
{
    tdstVector4D result;
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
VECTOR4 tdstVector4D vector4_mul(const tdstVector4D a, const tdstVector4D b)
{
    tdstVector4D result;
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
VECTOR4 tdstVector4D vector4_div(const tdstVector4D a, const tdstVector4D b)
{
    tdstVector4D result;
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
VECTOR4 tdstVector4D vector4_mulf(const tdstVector4D vector, const float value)
{
    tdstVector4D result;
    result.x = vector.x * value;
    result.y = vector.y * value;
    result.z = vector.z * value;
    result.w = vector.w * value;
    
    return result;
}
