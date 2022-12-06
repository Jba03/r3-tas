/*******************************************************
 * matrix3.h: 3x3 matrix
 *******************************************************
 * Created by Jba03 on 2022-11-26
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef matrix3_h
#define matrix3_h

#include "macrotricks.h"
#include "vector3.h"
#include "vector4.h"

#define MATRIX3
MATRIX3 MATRIX_DEFINITION(float, 3)

/**
 * matrix3_identity: Identity matrix
 */
extern const struct Matrix3 matrix3_identity;

/**
 * matrix3_read:
 * Read matrix from memory
 */
MATRIX3 struct Matrix3 matrix3_read(const address addr);

/**
 * matrix3_new:
 * Constructs a matrix with three rows
 */
MATRIX3 struct Matrix3 matrix3_new(const struct Vector3 row0, const struct Vector3 row1, const struct Vector3 row2);

#endif /* matrix3_h */
