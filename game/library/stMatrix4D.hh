#ifndef stMatrix4D_h
#define stMatrix4D_h

#include <assert.h>

#include "library.hh"
#include "mathc.hh"
#include "macrotricks.hh"
#include "stVector3D.hh"
#include "stVector4D.hh"
#include "assert.h"

#define MATRIX4

typedef union stMatrix4D stMatrix4D;

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
        stVector4D row0;
        stVector4D row1;
        stVector4D row2;
        stVector4D row3;
    };
    
    struct
    {
        float m[16];
    };
    
    static stMatrix4D identity()
    {
        return stMatrix4D(1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    stMatrix4D() { *this = identity(); }
    stMatrix4D(float m00, float m01, float m02, float m03,
               float m10, float m11, float m12, float m13,
               float m20, float m21, float m22, float m23,
               float m30, float m31, float m32, float m33):
    m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13),
    m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31), m32(m32), m33(m33) { }
    
    stMatrix4D hostByteOrder()
    {
        stMatrix4D result = identity();
        for (int i = 0; i < 16; i++)
            result.m[i] = host_byteorder_f32(*((uint32_t*)&m + i));
        
        return result;
    }
    
    
    static stMatrix4D perspective(const float fov_y, const float aspect, const float near_z, const float far_z)
    {
        stMatrix4D result = identity();
        
        const float ct = 1.0f / tanf(fov_y / 2.0f);
        
        result.m00 = ct / aspect;
        result.m11 = ct;
        result.m22 = (far_z + near_z) / (near_z - far_z);
        result.m23 = -1.0f;
        result.m32 = (2.0f * far_z * near_z) / (near_z - far_z);
        result.m33 = 0.0f;
        
        return result;
    }
    
    stMatrix4D operator *(stMatrix4D mm)
    {
        stMatrix4D r;
        
        r.m[0]  = m[0] * mm.m[0]  + m[4] * mm.m[1]  + m[8] * mm.m[2]   + m[12] * mm.m[3];
        r.m[4]  = m[0] * mm.m[4]  + m[4] * mm.m[5]  + m[8] * mm.m[6]   + m[12] * mm.m[7];
        r.m[8]  = m[0] * mm.m[8]  + m[4] * mm.m[9]  + m[8] * mm.m[10]  + m[12] * mm.m[11];
        r.m[12] = m[0] * mm.m[12] + m[4] * mm.m[13] + m[8] * mm.m[14]  + m[12] * mm.m[15];
        
        r.m[1]  = m[1] * mm.m[0]  + m[5] * mm.m[1]  + m[9] * mm.m[2]   + m[13] * mm.m[3];
        r.m[5]  = m[1] * mm.m[4]  + m[5] * mm.m[5]  + m[9] * mm.m[6]   + m[13] * mm.m[7];
        r.m[9]  = m[1] * mm.m[8]  + m[5] * mm.m[9]  + m[9] * mm.m[10]  + m[13] * mm.m[11];
        r.m[13] = m[1] * mm.m[12] + m[5] * mm.m[13] + m[9] * mm.m[14]  + m[13] * mm.m[15];
        
        r.m[2]  = m[2] * mm.m[0]  + m[6] * mm.m[1]  + m[10] * mm.m[2]  + m[14] * mm.m[3];
        r.m[6]  = m[2] * mm.m[4]  + m[6] * mm.m[5]  + m[10] * mm.m[6]  + m[14] * mm.m[7];
        r.m[10] = m[2] * mm.m[8]  + m[6] * mm.m[9]  + m[10] * mm.m[10] + m[14] * mm.m[11];
        r.m[14] = m[2] * mm.m[12] + m[6] * mm.m[13] + m[10] * mm.m[14] + m[14] * mm.m[15];
        
        r.m[3]  = m[3] * mm.m[0]  + m[7] * mm.m[1]  + m[11] * mm.m[2]  + m[15] * mm.m[3];
        r.m[7]  = m[3] * mm.m[4]  + m[7] * mm.m[5]  + m[11] * mm.m[6]  + m[15] * mm.m[7];
        r.m[11] = m[3] * mm.m[8]  + m[7] * mm.m[9]  + m[11] * mm.m[10] + m[15] * mm.m[11];
        r.m[15] = m[3] * mm.m[12] + m[7] * mm.m[13] + m[11] * mm.m[14] + m[15] * mm.m[15];
        
        return r;
    }
    
};

/**
 * matrix4_identity: Identity matrix
 */
static const stMatrix4D matrix4_identity =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

MATRIX4 stMatrix4D matrix4_host_byteorder(stMatrix4D m);

/**
 * matrix4_new:
 * Constructs a matrix with four rows
 */
MATRIX4 stMatrix4D matrix4_new(const stVector4D row0, const stVector4D row1, const stVector4D row2, const stVector4D row3);

/**
 * matrix4_mul:
 * Multiplies two matrices
 */
MATRIX4 stMatrix4D matrix4_mul(const stMatrix4D a, const stMatrix4D b);

/**
 * matrix4_inverse:
 * Returns the inverse of matrix `m`
 */
MATRIX4 stMatrix4D matrix4_inverse(const stMatrix4D m);

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
MATRIX4 stMatrix4D matrix4_transpose(const stMatrix4D m);


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
MATRIX4 stMatrix4D matrix4_make_translation(const float x, const float y, const float z);


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
MATRIX4 stMatrix4D matrix4_make_rotation_x(const float radians);

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
MATRIX4 stMatrix4D matrix4_make_rotation_y(const float radians);


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
MATRIX4 stMatrix4D matrix4_make_rotation_z(const float radians);

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
MATRIX4 stMatrix4D matrix4_make_scale(const float x, const float y, const float z);

/**
 * matrix4_make_scale_vector3:
 * Construct a 4x4 linear scale matrix from stVector3D values
 *
 * .____________________.
 * |  v.x  0    0    0  |
 * |  0    v.y  0    0  |
 * |  0    0    v.z  0  |
 * |  0    0    0    1  |
 * '————————————————————'
 */
MATRIX4 stMatrix4D matrix4_make_scale_vector3(const stVector3D v);

/**
 * matrix4_perspective:
 * Creates a camera perspective projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 stMatrix4D matrix4_perspective(const float fov_y, const float aspect, const float near_z, const float far_z);

/**
 * matrix4_ortho:
 * Creates a camera orthogonal projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 stMatrix4D matrix4_ortho(const float left, const float right, const float bottom, const float top, const float near, const float far);

/**
 * matrix4_lookat:
 * Constructs a camera view matrix
 */
MATRIX4 stMatrix4D matrix4_lookat(const stVector3D eye, const stVector3D center, const stVector3D up);

/**
 * vector4_mul_matrix4:
 * Multiply a four-component vector with a 4x4 matrix
 */
VECTOR4 stVector4D vector4_mul_matrix4(const stVector4D v, const stMatrix4D m);

#define game_matrix4_position(m) vector3_new(m.m30, m.m31, m.m32)
#define game_matrix4_scale(m) vector3_new(m.m31, m.m32, m.m32)
#define game_matrix4_rotation_z(m) atan2(m.m01, m.m02)

#endif /* stMatrix4D_h */
