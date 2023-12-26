#ifndef stVector4D_h
#define stVector4D_h

#include "library.hh"

#define VECTOR4

struct stVector4D
{
    float32 x;
    float32 y;
    float32 z;
    float32 w;
    
    stVector4D() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    stVector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
};

VECTOR4 stVector4D vector4_host_byteorder(const stVector4D v);

/**
 * vector4_new:
 * Construct a new four-component vector
 */
VECTOR4 stVector4D vector4_new(const float x, const float y, const float z, const float w);

/**
 * vector4_add:
 * Add two four-component vectors
 */
VECTOR4 stVector4D vector4_add(const stVector4D a, const stVector4D b);

/**
 * vector4_sub:
 * Subtract four-component vectors
 */
VECTOR4 stVector4D vector4_sub(const stVector4D a, const stVector4D b);

/**
 * vector4_mul:
 * Multiply two four-component vectors
 */
VECTOR4 stVector4D vector4_mul(const stVector4D a, const stVector4D b);

/**
 * vector4_div:
 * Divide four-component vector `a` by `b`
 */
VECTOR4 stVector4D vector4_div(const stVector4D a, const stVector4D b);

/**
 * vector4_mulf:
 * Multiply a four-component vector with a floating-point scalar value
 */
VECTOR4 stVector4D vector4_mulf(const stVector4D vector, const float value);


#endif /* stVector4D_h */
