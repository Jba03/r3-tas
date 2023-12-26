#ifndef stVector2D_h
#define stVector2D_h

#include "mathc.hh"

#define VECTOR2

struct stVector2D
{
    float x;
    float y;
};

VECTOR2 stVector2D vector2_host_byteorder(const stVector2D v);

/**
 * vector2_new:
 * Creates a new two-component vector
 */
VECTOR2 stVector2D vector2_new(const float x, const float y);

/**
 * vector2_add:
 * Add two vectors
 */
VECTOR2 stVector2D vector2_add(const stVector2D a, const stVector2D b);

/**
 * vector2_sub:
 * Subtract vector `b` from vector `a`
 */
VECTOR2 stVector2D vector2_sub(const stVector2D a, const stVector2D b);

/**
 * vector2_mul:
 * Multiply two vectors
 */
VECTOR2 stVector2D vector2_mul(const stVector2D a, const stVector2D b);

/**
 * vector2_mulf:
 * Multiply two vectors
 */
VECTOR2 stVector2D vector2_mulf(const stVector2D a, const float value);

/**
 * vector2_div:
 * Divide vector `a` by vector `b`
 */
VECTOR2 stVector2D vector2_div(const stVector2D a, const stVector2D b);

/**
 * vector2_negate:
 * Negate a vector
 */
VECTOR2 stVector2D vector2_negate(const stVector2D vector);

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 float vector2_dot(const stVector2D a, const stVector2D b);

/**
 * vector2_dot:
 * Return the dot product of `a` and `b`
 */
VECTOR2 float vector2_length(const stVector2D a);

#endif /* stVector2D_h */
