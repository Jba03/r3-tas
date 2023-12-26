#ifndef stVector3D_h
#define stVector3D_h

#include "library.hh"

#include <math.h>
#include <stdlib.h>

#define VECTOR3

typedef struct stVector3D stVector3D;

struct stVector3D {
    float x;
    float y;
    float z;
    
    //operator *(stVector3D other) { };
};

VECTOR3 stVector3D vector3_host_byteorder(const stVector3D v);

/**
 * stVector3D_new:
 * Construct a new three-component vector
 */
VECTOR3 stVector3D vector3_new(const float x, const float y, const float z);

/**
 * stVector3D_add:
 * Add two vectors
 */
VECTOR3 stVector3D vector3_add(const stVector3D a, const stVector3D b);

/**
 * stVector3D_sub:
 * Subtracts vector `b` from vector `a`
 */
VECTOR3 stVector3D vector3_sub(const stVector3D a, const stVector3D b);

/**
 * stVector3D_mul:
 * Multiplies vector `a` with vector `b`
 */
VECTOR3 stVector3D vector3_mul(const stVector3D a, const stVector3D b);

/**
 * stVector3D_div:
 * Divides vector `a` with vector `b`
 */
VECTOR3 stVector3D vector3_div(const stVector3D a, const stVector3D b);

/**
 * stVector3D_negate:
 * Negate a vector
 */
VECTOR3 stVector3D vector3_negate(const stVector3D vector);

/**
 * stVector3D_mulf:
 * Multiply a vector by a scalar float value
 */
VECTOR3 stVector3D vector3_mulf(const stVector3D vector, const float value);

/**
 * stVector3D_dot:
 * Computes the dot product a•b of two vectors `a` and `b`
 */
VECTOR3 const float vector3_dot(const stVector3D a, const stVector3D b);

/**
 * stVector3D_cross:
 * Calculate the vector cross product. The cross product of two vectors
 * is a vector that is perpendicular (therefore normal) to the two vectors.
 */
VECTOR3 stVector3D vector3_cross(const stVector3D a, const stVector3D b);

/**
 * stVector3D_wedge:
 * I have no idea what this does.
 */
VECTOR3 stVector3D vector3_wedge(const stVector3D a, const stVector3D b);

/**
 * stVector3D_length:
 * Computes the length of a vector, i.e. the square root of
 * the total sum of the vector's squared components.
 */
VECTOR3 const float vector3_length(const stVector3D vector);

/**
 * stVector3D_normalize:
 * Normalizes a 3-component vector. In case the input
 * vector has a length of zero, the input vector is returned.
 *
 * v = v * 1 / |v|
 */
VECTOR3 stVector3D vector3_normalize(const stVector3D vector);

/**
 * stVector3D_random:
 * Returns a random 3-component unit vector
 */
VECTOR3 stVector3D vector3_random(void);

#endif /* stVector3D_h */
