#ifndef stMatrix4D_h
#define stMatrix4D_h

#include <assert.h>

#include "structure.h"
#include "mathc.h"
#include "macrotricks.h"
#include "stVector3D.h"
#include "stVector4D.h"
#include "assert.h"

#define MATRIX4

typedef union stMatrix4D tdstMatrix4D;

union stMatrix4D
{
    struct
    {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
    };
    
    struct
    {
        tdstVector4D row0;
        tdstVector4D row1;
        tdstVector4D row2;
        tdstVector4D row3;
    };
    
    struct
    {
        float m[16];
    };
};

/**
 * matrix4_identity: Identity matrix
 */
static const tdstMatrix4D matrix4_identity =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

MATRIX4 tdstMatrix4D matrix4_host_byteorder(tdstMatrix4D m);

/**
 * matrix4_new:
 * Constructs a matrix with four rows
 */
MATRIX4 tdstMatrix4D matrix4_new(const tdstVector4D row0, const tdstVector4D row1, const tdstVector4D row2, const tdstVector4D row3);

/**
 * matrix4_mul:
 * Multiplies two matrices
 */
MATRIX4 tdstMatrix4D matrix4_mul(const tdstMatrix4D a, const tdstMatrix4D b);

/**
 * matrix4_inverse:
 * Returns the inverse of matrix `m`
 */
MATRIX4 tdstMatrix4D matrix4_inverse(const tdstMatrix4D m);

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
MATRIX4 tdstMatrix4D matrix4_transpose(const tdstMatrix4D m);


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
MATRIX4 tdstMatrix4D matrix4_make_translation(const float x, const float y, const float z);


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
MATRIX4 tdstMatrix4D matrix4_make_rotation_x(const float radians);

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
MATRIX4 tdstMatrix4D matrix4_make_rotation_y(const float radians);


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
MATRIX4 tdstMatrix4D matrix4_make_rotation_z(const float radians);

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
MATRIX4 tdstMatrix4D matrix4_make_scale(const float x, const float y, const float z);

/**
 * matrix4_make_scale_vector3:
 * Construct a 4x4 linear scale matrix from tdstVector3D values
 *
 * .____________________.
 * |  v.x  0    0    0  |
 * |  0    v.y  0    0  |
 * |  0    0    v.z  0  |
 * |  0    0    0    1  |
 * '————————————————————'
 */
MATRIX4 tdstMatrix4D matrix4_make_scale_vector3(const tdstVector3D v);

/**
 * matrix4_perspective:
 * Creates a camera perspective projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 tdstMatrix4D matrix4_perspective(const float fov_y, const float aspect, const float near_z, const float far_z);

/**
 * matrix4_ortho:
 * Creates a camera orthogonal projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 tdstMatrix4D matrix4_ortho(const float left, const float right, const float bottom, const float top, const float near, const float far);

/**
 * matrix4_lookat:
 * Constructs a camera view matrix
 */
MATRIX4 tdstMatrix4D matrix4_lookat(const tdstVector3D eye, const tdstVector3D center, const tdstVector3D up);

/**
 * vector4_mul_matrix4:
 * Multiply a four-component vector with a 4x4 matrix
 */
VECTOR4 tdstVector4D vector4_mul_matrix4(const tdstVector4D v, const tdstMatrix4D m);

#define game_matrix4_position(m) vector3_new(m.m30, m.m31, m.m32)
#define game_matrix4_scale(m) vector3_new(m.m31, m.m32, m.m32)
#define game_matrix4_rotation_z(m) atan2(m.m01, m.m02)

#endif /* stMatrix4D_h */
