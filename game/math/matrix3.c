/*******************************************************
 * matrix3.c: 3x3 matrix operations
 *******************************************************
 * Created by Jba03 on 2022-11-26
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#include "matrix3.h"
#include "memory.h"

#include <assert.h>
#include <math.h>

#pragma mark - Matrix3

const struct Matrix3 matrix3_identity =
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

MATRIX3 struct Matrix3 matrix3_read(const address addr)
{
    struct Matrix3 m = matrix3_identity;
    m.offset = addr;
    
    for (int i = 0; i < 9; i++)
        m.m[i] = memory.read_float(addr + 3 * i);
    return m;
}

MATRIX3 struct Matrix3 matrix3_new(const struct Vector3 row0, const struct Vector3 row1, const struct Vector3 row2)
{
    struct Matrix3 m;
    m.row0 = row0;
    m.row1 = row1;
    m.row2 = row2;
    
    return m;
}
