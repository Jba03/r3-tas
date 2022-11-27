/*******************************************************
 * vector2.h: Two-component vector
 *******************************************************
 * Created by Jba03 on 2021-05-22
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef vector2_h
#define vector2_h

#include "memory.h"
#include "macrotricks.h"

#define VECTOR2
VECTOR2 VECTOR_DEFINITION(float, 2)

/**
 * vector2_new:
 * Creates a new two-component vector
 */
VECTOR2 struct Vector2 vector2_new(const float x, const float y);

/**
 * vector2_add:
 * Add two vectors
 */
VECTOR2 struct Vector2 vector2_add(const struct Vector2 a, const struct Vector2 b);

/**
 * vector2_sub:
 * Subtract vector `b` from vector `a`
 */
VECTOR2 struct Vector2 vector2_sub(const struct Vector2 a, const struct Vector2 b);

/**
 * vector2_mul:
 * Multiply two vectors
 */
VECTOR2 struct Vector2 vector2_mul(const struct Vector2 a, const struct Vector2 b);

/**
 * vector2_mulf:
 * Multiply two vectors
 */
VECTOR2 struct Vector2 vector2_mulf(const struct Vector2 a, const float value);

/**
 * vector2_div:
 * Divide vector `a` by vector `b`
 */
VECTOR2 struct Vector2 vector2_div(const struct Vector2 a, const struct Vector2 b);

/**
 * vector2_negate:
 * Negate a vector
 */
VECTOR2 struct Vector2 vector2_negate(const struct Vector2 vector);

#endif /* vector2_h */
