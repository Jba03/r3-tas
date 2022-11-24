/*******************************************************
 * matrix4.h: 4x4 matrix
 *******************************************************
 * Created by Jba03 on 2021-05-20
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#ifndef matrix4_h
#define matrix4_h

#include "macrotricks.h"
#include "vector3.h"
#include "vector4.h"

#define MATRIX4
MATRIX4 MATRIX_DEFINITION(float, 4)

/**
 * matrix4_identity: Identity matrix
 */
extern const struct Matrix4 matrix4_identity;

/**
 * matrix4_read:
 * Read a matrix from memory
 */
MATRIX4 struct Matrix4 matrix4_read(const address addr);

/**
 * matrix4_new:
 * Constructs a matrix with four rows
 */
MATRIX4 struct Matrix4 matrix4_new(const struct Vector4 row0, const struct Vector4 row1, const struct Vector4 row2, const struct Vector4 row3);

/**
 * matrix4_mul:
 * Multiplies two matrices
 */
MATRIX4 struct Matrix4 matrix4_mul(const struct Matrix4 a, const struct Matrix4 b);

/**
 * matrix4_inverse:
 * Returns the inverse of matrix `m`
 */
MATRIX4 struct Matrix4 matrix4_inverse(const struct Matrix4 m);

/**
 * matrix4_transpose:
 * Return the transpose of 4x4 matrix `m`
 *
 * .____________________.     .____________________.
 * | m00  m01  m02  m03 |     | m00  m10  m20  m30 |
 * | m10  m11  m12  m13 | --> | m01  m11  m21  m31 |
 * | m20  m21  m22  m23 |     | m02  m12  m22  m32 |
 * | m30  m31  m32  m33 |     | m03  m13  m23  m33 |
 * '————————————————————'     '————————————————————'
 */
MATRIX4 struct Matrix4 matrix4_transpose(const struct Matrix4 m);

#pragma mark - Translation, scale and rotation

/**
 * matrix4_translation:
 * Create a 4x4 translation matrix
 *
 * ._________________.
 * |  1   0   0  .x  |
 * |  0   1   0  .y  |
 * |  0   0   1  .z  |
 * |  0   0   0   1  |
 * '—————————————————'
 */
MATRIX4 struct Matrix4 matrix4_make_translation(const float x, const float y, const float z);


/**
 * matrix4_rotation_x:
 * Construct a 4x4 X-axis rotation matrix
 *
 * ._______________________.
 * |  1   0      0      0  |
 * |  0   cos α  sin α  0  |
 * |  0  -sin α  cos α  0  |
 * |  0   0      0      1  |
 * '———————————————————————'
 */
MATRIX4 struct Matrix4 matrix4_make_rotation_x(const float radians);

/**
 * matrix4_rotation_y:
 * Construct a 4x4 Y-axis rotation matrix
 *
 * ._______________________.
 * |  cos α  0  -sin α  0  |
 * |  0      1   0      0  |
 * |  sin α  0   cos α  0  |
 * |  0      0   0      1  |
 * '———————————————————————'
 */
MATRIX4 struct Matrix4 matrix4_make_rotation_y(const float radians);


/**
 * matrix4_rotation_z:
 * Construct a 4x4 Z-axis rotation matrix
 *
 * .______________________.
 * |  cos α  sin α  0  0  |
 * | -sin α  cos α  0  0  |
 * |  0      0      1  0  |
 * |  0      0      0  1  |
 * '——————————————————————'
 */
MATRIX4 struct Matrix4 matrix4_make_rotation_z(const float radians);

/**
 * matrix4_make_scale:
 * Construct a 4x4 linear scale matrix
 *
 * ._________________.
 * | .sx  0   0   0  |
 * |  0  .sy  0   0  |
 * |  0   0  .sz  0  |
 * |  0   0   0   1  |
 * '—————————————————'
 */
MATRIX4 struct Matrix4 matrix4_make_scale(const float x, const float y, const float z);

/**
 * matrix4_make_scale_vector3:
 * Construct a 4x4 linear scale matrix from Vector3 values
 *
 * .____________________.
 * |  v.x  0    0    0  |
 * |  0    v.y  0    0  |
 * |  0    0    v.z  0  |
 * |  0    0    0    1  |
 * '————————————————————'
 */
MATRIX4 struct Matrix4 matrix4_make_scale_vector3(const struct Vector3 v);

/**
 * matrix4_perspective:
 * Creates a camera perspective projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 struct Matrix4 matrix4_perspective(const float fov_y, const float aspect, const float near_z, const float far_z);

/**
 * matrix4_ortho:
 * Creates a camera orthogonal projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 struct Matrix4 matrix4_ortho(const float left, const float right, const float bottom, const float top, const float near, const float far);

/**
 * matrix4_lookat:
 * Constructs a camera view matrix
 */
MATRIX4 struct Matrix4 matrix4_lookat(const struct Vector3 eye, const struct Vector3 center, const struct Vector3 up);

/**
 * vector4_mul_matrix4:
 * Multiply a four-component vector with a 4x4 matrix
 */
VECTOR4 struct Vector4 vector4_mul_matrix4(const struct Vector4 v, const struct Matrix4 m);

#endif /* matrix4_h */
