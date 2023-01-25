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

#define MATRIX3
MATRIX3 MATRIX_DEFINITION(float, 3)

/**
 * matrix3_identity: Identity matrix
 */
static const struct matrix3 matrix3_identity =
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

/**
 * matrix3_new:
 * Constructs a matrix with three rows
 */
MATRIX3 struct matrix3 matrix3_new(const struct vector3 row0, const struct vector3 row1, const struct vector3 row2)
{
    struct matrix3 m;
    m.row0 = row0;
    m.row1 = row1;
    m.row2 = row2;
    
    return m;
}

#endif /* matrix3_h */
