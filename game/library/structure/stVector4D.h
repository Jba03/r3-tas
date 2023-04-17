#ifndef stVector4D_h
#define stVector4D_h

#include "structure.h"

#define VECTOR4

typedef struct stVector4D tdstVector4D;

struct stVector4D
{
    float x;
    float y;
    float z;
    float w;
};

VECTOR4 tdstVector4D vector4_host_byteorder(const tdstVector4D v);

/**
 * vector4_new:
 * Construct a new four-component vector
 */
VECTOR4 tdstVector4D vector4_new(const float x, const float y, const float z, const float w);

/**
 * vector4_add:
 * Add two four-component vectors
 */
VECTOR4 tdstVector4D vector4_add(const tdstVector4D a, const tdstVector4D b);

/**
 * vector4_sub:
 * Subtract four-component vectors
 */
VECTOR4 tdstVector4D vector4_sub(const tdstVector4D a, const tdstVector4D b);

/**
 * vector4_mul:
 * Multiply two four-component vectors
 */
VECTOR4 tdstVector4D vector4_mul(const tdstVector4D a, const tdstVector4D b);

/**
 * vector4_div:
 * Divide four-component vector `a` by `b`
 */
VECTOR4 tdstVector4D vector4_div(const tdstVector4D a, const tdstVector4D b);

/**
 * vector4_mulf:
 * Multiply a four-component vector with a floating-point scalar value
 */
VECTOR4 tdstVector4D vector4_mulf(const tdstVector4D vector, const float value);


#endif /* stVector4D_h */
