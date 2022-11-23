/*******************************************************
 * vector3.h: Three-component vector
 *******************************************************
 * Created by Jba03 on 2021-05-20
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef vector3_h
#define vector3_h

#include "macrotricks.h"
#include "memory.h"

#define VECTOR3
VECTOR3 VECTOR_DEFINITION(float, 3)

/**
 * vector3_read:
 * Read a three-component vector from memory
 */
VECTOR3 union Vector3 vector3_read(const address addr);

/**
 * vector3_new:
 * Creates a new three-component vector
 */
VECTOR3 union Vector3 vector3_new(const float x, const float y, const float z);

/**
 * vector3_add:
 * Add two vectors
 */
VECTOR3 union Vector3 vector3_add(union Vector3 a, union Vector3 b);

/**
 * vector3_sub:
 * Subtracts vector `b` from vector `a`
 */
VECTOR3 union Vector3 vector3_sub(union Vector3 a, union Vector3 b);

/**
 * vector3_mul:
 * Multiplies vector `a` with vector `b`
 */
VECTOR3 union Vector3 vector3_mul(union Vector3 a, union Vector3 b);

/**
 * vector3_div:
 * Divides vector `a` with vector `b`
 */
VECTOR3 union Vector3 vector3_div(union Vector3 a, union Vector3 b);

/**
 * vector3_negate:
 * Negate a vector
 */
VECTOR3 union Vector3 vector3_negate(union Vector3 vector);

/**
 * vector3_mulf:
 * Multiply a vector by a scalar float value
 */
VECTOR3 union Vector3 vector3_mulf(union Vector3 vector, const float value);

/**
 * vector3_dot:
 * Computes the dot product a•b of two vectors `a` and `b`
 */
VECTOR3 const float vector3_dot(union Vector3 a, union Vector3 b);

/**
 * vector3_cross:
 * Calculate the vector cross product. The cross product of two vectors
 * is a vector that is perpendicular (therefore normal) to the two vectors.
 */
VECTOR3 union Vector3 vector3_cross(union Vector3 a, union Vector3 b);

/**
 * vector3_wedge:
 * I have no idea what this does.
 */
VECTOR3 union Vector3 vector3_wedge(union Vector3 a, union Vector3 b);

/**
 * vector3_length:
 * Computes the length of a vector, i.e. the square root of
 * the total sum of the vector's squared components.
 */
VECTOR3 const float vector3_length(const union Vector3 vector);

/**
 * vector3_normalize:
 * Normalizes a 3-component vector. In case the input
 * vector has a length of zero, it is returned.
 *
 * v = v * 1 / |v|
 */
VECTOR3 union Vector3 vector3_normalize(const union Vector3 vector);

/**
 * vector3_random:
 * Returns a random 3-component unit vector
 */
VECTOR3 union Vector3 vector3_random(void);

#endif /* vector3_h */
