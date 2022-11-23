/*******************************************************
 * vector4.h: Four-component vector
 *******************************************************
 * Created by Jba03 on 2021-05-20
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef vector4_h
#define vector4_h

#include "macrotricks.h"
#include "memory.h"

#define VECTOR4
VECTOR4 VECTOR_DEFINITION(float, 4)

/**
 * vector4_read:
 * Read a four-component vector from memory
 */
VECTOR4 union Vector4 vector4_read(const address addr);

/**
 * vector4_new:
 * Creates a new four-component vector
 */
VECTOR4 union Vector4 vector4_new(const float x, const float y, const float z, const float w);

/**
 * vector4_add:
 * Add two four-component vectors
 */
VECTOR4 union Vector4 vector4_add(const union Vector4 a, const union Vector4 b);

/**
 * vector4_sub:
 * Subtract four-component vectors
 */
VECTOR4 union Vector4 vector4_sub(const union Vector4 a, const union Vector4 b);

/**
 * vector4_mul:
 * Multiply two four-component vectors
 */
VECTOR4 union Vector4 vector4_mul(const union Vector4 a, const union Vector4 b);

/**
 * vector4_div:
 * Divide four-component vector `a` by `b`
 */
VECTOR4 union Vector4 vector4_div(const union Vector4 a, const union Vector4 b);

/**
 * vector4_mulf:
 * Multiply a four-component vector with a floating-point scalar value
 */
VECTOR4 union Vector4 vector4_mulf(const union Vector4 vector, const float value);

#endif /* vector4_h */
