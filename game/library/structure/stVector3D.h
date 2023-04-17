#ifndef stVector3D_h
#define stVector3D_h

#include "structure.h"

#include <math.h>
#include <stdlib.h>

#define VECTOR3

typedef struct stVector3D tdstVector3D;

struct stVector3D
{
    float x;
    float y;
    float z;
};

VECTOR3 tdstVector3D vector3_host_byteorder(const tdstVector3D v);

/**
 * tdstVector3D_new:
 * Construct a new three-component vector
 */
VECTOR3 tdstVector3D vector3_new(const float x, const float y, const float z);

/**
 * tdstVector3D_add:
 * Add two vectors
 */
VECTOR3 tdstVector3D vector3_add(const tdstVector3D a, const tdstVector3D b);

/**
 * tdstVector3D_sub:
 * Subtracts vector `b` from vector `a`
 */
VECTOR3 tdstVector3D vector3_sub(const tdstVector3D a, const tdstVector3D b);

/**
 * tdstVector3D_mul:
 * Multiplies vector `a` with vector `b`
 */
VECTOR3 tdstVector3D vector3_mul(const tdstVector3D a, const tdstVector3D b);

/**
 * tdstVector3D_div:
 * Divides vector `a` with vector `b`
 */
VECTOR3 tdstVector3D vector3_div(const tdstVector3D a, const tdstVector3D b);

/**
 * tdstVector3D_negate:
 * Negate a vector
 */
VECTOR3 tdstVector3D vector3_negate(const tdstVector3D vector);

/**
 * tdstVector3D_mulf:
 * Multiply a vector by a scalar float value
 */
VECTOR3 tdstVector3D vector3_mulf(const tdstVector3D vector, const float value);

/**
 * tdstVector3D_dot:
 * Computes the dot product a•b of two vectors `a` and `b`
 */
VECTOR3 const float vector3_dot(const tdstVector3D a, const tdstVector3D b);

/**
 * tdstVector3D_cross:
 * Calculate the vector cross product. The cross product of two vectors
 * is a vector that is perpendicular (therefore normal) to the two vectors.
 */
VECTOR3 tdstVector3D vector3_cross(const tdstVector3D a, const tdstVector3D b);

/**
 * tdstVector3D_wedge:
 * I have no idea what this does.
 */
VECTOR3 tdstVector3D vector3_wedge(const tdstVector3D a, const tdstVector3D b);

/**
 * tdstVector3D_length:
 * Computes the length of a vector, i.e. the square root of
 * the total sum of the vector's squared components.
 */
VECTOR3 const float vector3_length(const tdstVector3D vector);

/**
 * tdstVector3D_normalize:
 * Normalizes a 3-component vector. In case the input
 * vector has a length of zero, the input vector is returned.
 *
 * v = v * 1 / |v|
 */
VECTOR3 tdstVector3D vector3_normalize(const tdstVector3D vector);

/**
 * tdstVector3D_random:
 * Returns a random 3-component unit vector
 */
VECTOR3 tdstVector3D vector3_random(void);

#endif /* stVector3D_h */
