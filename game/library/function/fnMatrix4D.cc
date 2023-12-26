//
//  fnMatrix4D.c
//  r3-tas
//
//  Created by Jba03 on 2023-04-17.
//

#include "stMatrix4D.hh"
#include "memory.hh"

MATRIX4 stMatrix4D matrix4_host_byteorder(stMatrix4D m)
{
    stMatrix4D result = matrix4_identity;
    for (int i = 0; i < 16; i++)
        result.m[i] = host_byteorder_f32(*((uint32_t*)&m.m + i));
    
    return result;
}

/**
 * matrix4_new:
 * Constructs a matrix with four rows
 */
MATRIX4 stMatrix4D matrix4_new(const stVector4D row0, const stVector4D row1, const stVector4D row2, const stVector4D row3)
{
    stMatrix4D m;
    m.row0 = row0;
    m.row1 = row1;
    m.row2 = row2;
    m.row3 = row3;
    
    return m;
}

/**
 * matrix4_mul:
 * Multiplies two matrices
 */
MATRIX4 stMatrix4D matrix4_mul(const stMatrix4D a, const stMatrix4D b)
{
    stMatrix4D m;
    
    m.m[0]  = a.m[0] * b.m[0]  + a.m[4] * b.m[1]  + a.m[8] * b.m[2]   + a.m[12] * b.m[3];
    m.m[4]  = a.m[0] * b.m[4]  + a.m[4] * b.m[5]  + a.m[8] * b.m[6]   + a.m[12] * b.m[7];
    m.m[8]  = a.m[0] * b.m[8]  + a.m[4] * b.m[9]  + a.m[8] * b.m[10]  + a.m[12] * b.m[11];
    m.m[12] = a.m[0] * b.m[12] + a.m[4] * b.m[13] + a.m[8] * b.m[14]  + a.m[12] * b.m[15];
    
    m.m[1]  = a.m[1] * b.m[0]  + a.m[5] * b.m[1]  + a.m[9] * b.m[2]   + a.m[13] * b.m[3];
    m.m[5]  = a.m[1] * b.m[4]  + a.m[5] * b.m[5]  + a.m[9] * b.m[6]   + a.m[13] * b.m[7];
    m.m[9]  = a.m[1] * b.m[8]  + a.m[5] * b.m[9]  + a.m[9] * b.m[10]  + a.m[13] * b.m[11];
    m.m[13] = a.m[1] * b.m[12] + a.m[5] * b.m[13] + a.m[9] * b.m[14]  + a.m[13] * b.m[15];
    
    m.m[2]  = a.m[2] * b.m[0]  + a.m[6] * b.m[1]  + a.m[10] * b.m[2]  + a.m[14] * b.m[3];
    m.m[6]  = a.m[2] * b.m[4]  + a.m[6] * b.m[5]  + a.m[10] * b.m[6]  + a.m[14] * b.m[7];
    m.m[10] = a.m[2] * b.m[8]  + a.m[6] * b.m[9]  + a.m[10] * b.m[10] + a.m[14] * b.m[11];
    m.m[14] = a.m[2] * b.m[12] + a.m[6] * b.m[13] + a.m[10] * b.m[14] + a.m[14] * b.m[15];
    
    m.m[3]  = a.m[3] * b.m[0]  + a.m[7] * b.m[1]  + a.m[11] * b.m[2]  + a.m[15] * b.m[3];
    m.m[7]  = a.m[3] * b.m[4]  + a.m[7] * b.m[5]  + a.m[11] * b.m[6]  + a.m[15] * b.m[7];
    m.m[11] = a.m[3] * b.m[8]  + a.m[7] * b.m[9]  + a.m[11] * b.m[10] + a.m[15] * b.m[11];
    m.m[15] = a.m[3] * b.m[12] + a.m[7] * b.m[13] + a.m[11] * b.m[14] + a.m[15] * b.m[15];
    
    return m;
}

/**
 * matrix4_inverse:
 * Returns the inverse of matrix `m`
 */
MATRIX4 stMatrix4D matrix4_inverse(const stMatrix4D m)
{
    stMatrix4D result;
    
    float s0 = m.m00 * m.m11 - m.m10 * m.m01;
    float s1 = m.m00 * m.m12 - m.m10 * m.m02;
    float s2 = m.m00 * m.m13 - m.m10 * m.m03;
    float s3 = m.m01 * m.m12 - m.m11 * m.m02;
    float s4 = m.m01 * m.m13 - m.m11 * m.m03;
    float s5 = m.m02 * m.m13 - m.m12 * m.m03;
    
    float c5 = m.m22 * m.m33 - m.m32 * m.m23;
    float c4 = m.m21 * m.m33 - m.m31 * m.m23;
    float c3 = m.m21 * m.m32 - m.m31 * m.m22;
    float c2 = m.m20 * m.m33 - m.m30 * m.m23;
    float c1 = m.m20 * m.m32 - m.m30 * m.m22;
    float c0 = m.m20 * m.m31 - m.m30 * m.m21;
    
    float const det = (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
    float const invdet = 1.0f / det;
    
    /* If the determinant is zero,
     * the matrix is singular and cannot be inverted. */
    assert(det != 0.0f);
    
    result.m00 = ( m.m11 * c5 - m.m12 * c4 + m.m13 * c3) * invdet;
    result.m01 = (-m.m01 * c5 + m.m02 * c4 - m.m03 * c3) * invdet;
    result.m02 = ( m.m31 * s5 - m.m32 * s4 + m.m33 * s3) * invdet;
    result.m03 = (-m.m21 * s5 + m.m22 * s4 - m.m23 * s3) * invdet;
    
    result.m10 = (-m.m10 * c5 + m.m12 * c2 - m.m13 * c1) * invdet;
    result.m11 = ( m.m00 * c5 - m.m02 * c2 + m.m03 * c1) * invdet;
    result.m12 = (-m.m30 * s5 + m.m32 * s2 - m.m33 * s1) * invdet;
    result.m13 = ( m.m20 * s5 - m.m22 * s2 + m.m23 * s1) * invdet;
    
    result.m20 = ( m.m10 * c4 - m.m11 * c2 + m.m13 * c0) * invdet;
    result.m21 = (-m.m00 * c4 + m.m01 * c2 - m.m03 * c0) * invdet;
    result.m22 = ( m.m30 * s4 - m.m31 * s2 + m.m33 * s0) * invdet;
    result.m23 = (-m.m20 * s4 + m.m21 * s2 - m.m23 * s0) * invdet;
    
    result.m30 = (-m.m10 * c3 + m.m11 * c1 - m.m12 * c0) * invdet;
    result.m31 = ( m.m00 * c3 - m.m01 * c1 + m.m02 * c0) * invdet;
    result.m32 = (-m.m30 * s3 + m.m31 * s1 - m.m32 * s0) * invdet;
    result.m33 = ( m.m20 * s3 - m.m21 * s1 + m.m22 * s0) * invdet;
    
    return result;
}

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
MATRIX4 stMatrix4D matrix4_transpose(const stMatrix4D m)
{
    stMatrix4D result;
    
    result.m[0] = m.m[0];
    result.m[1] = m.m[4];
    result.m[2] = m.m[8];
    result.m[3] = m.m[12];
    
    result.m[4] = m.m[1];
    result.m[5] = m.m[5];
    result.m[6] = m.m[9];
    result.m[7] = m.m[13];
    
    result.m[8] = m.m[2];
    result.m[9] = m.m[6];
    result.m[10] = m.m[10];
    result.m[11] = m.m[14];
    
    result.m[12] = m.m[3];
    result.m[13] = m.m[7];
    result.m[14] = m.m[11];
    result.m[15] = m.m[15];
    
    return result;
}

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
MATRIX4 stMatrix4D matrix4_make_translation(const float x, const float y, const float z)
{
    stMatrix4D result = matrix4_identity;
    result.m03 = x;
    result.m13 = y;
    result.m23 = z;
    
    return result;
}


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
MATRIX4 stMatrix4D matrix4_make_rotation_x(const float radians)
{
    float sine = sin(radians);
    float cosine = cos(radians);
    
    stMatrix4D result = matrix4_identity;
    result.m11 = cosine;
    result.m12 = sine;
    result.m21 = -sine;
    result.m22 = cosine;
    
    return result;
}

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
MATRIX4 stMatrix4D matrix4_make_rotation_y(const float radians)
{
    const float sine = sin(radians);
    const float cosine = cos(radians);
    
    stMatrix4D result = matrix4_identity;
    result.m00 = cosine;
    result.m02 = -sine;
    result.m20 = sine;
    result.m22 = cosine;
    
    return result;
}


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
MATRIX4 stMatrix4D matrix4_make_rotation_z(const float radians)
{
    const float sine = sin(radians);
    const float cosine = cos(radians);
    
    stMatrix4D result = matrix4_identity;
    result.m00 = cosine;
    result.m01 = sine;
    result.m10 = -sine;
    result.m11 = cosine;
    
    return result;
}

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
MATRIX4 stMatrix4D matrix4_make_scale(const float x, const float y, const float z)
{
    stMatrix4D result = matrix4_identity;
    result.m00 = x;
    result.m11 = y;
    result.m22 = z;
    
    return result;
}

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
MATRIX4 stMatrix4D matrix4_make_scale_vector3(const stVector3D v)
{
    stMatrix4D result = matrix4_identity;
    result.m00 = v.x;
    result.m11 = v.y;
    result.m22 = v.z;
    
    return result;
}

/**
 * matrix4_perspective:
 * Creates a camera perspective projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 stMatrix4D matrix4_perspective(const float fov_y, const float aspect, const float near_z, const float far_z)
{
    stMatrix4D result = matrix4_identity;
    
    const float ct = 1.0f / tanf(fov_y / 2.0f);
    
    result.m00 = ct / aspect;
    result.m11 = ct;
    result.m22 = (far_z + near_z) / (near_z - far_z);
    result.m23 = -1.0f;
    result.m32 = (2.0f * far_z * near_z) / (near_z - far_z);
    result.m33 = 0.0f;
    
    return result;
}

/**
 * matrix4_ortho:
 * Creates a camera orthogonal projection matrix from field of view, aspect ratio, and near + far planes.
 */
MATRIX4 stMatrix4D matrix4_ortho(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    float ral = right + left;
    float rsl = right - left;
    float tab = top + bottom;
    float tsb = top - bottom;
    float fan = far + near;
    float fsn = far - near;
    
    stMatrix4D result =
    {
        2.0f / rsl, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / tsb, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / fsn, 0.0f,
        -ral / rsl, -tab / tsb, -fan / fsn, 1.0f
    };
    
    return result;
}

/**
 * matrix4_lookat:
 * Constructs a camera view matrix
 */
MATRIX4 stMatrix4D matrix4_lookat(const stVector3D eye, const stVector3D center, const stVector3D up)
{
    stVector3D n = vector3_normalize(vector3_add(eye, vector3_negate(center)));
    stVector3D u = vector3_normalize(vector3_cross(up, n));
    stVector3D v = vector3_cross(n, u);
    
    stMatrix4D result =
    {
        u.x, v.x, n.x, 0.0f,
        u.y, v.y, n.y, 0.0f,
        u.z, v.z, n.z, 0.0f,
        vector3_dot(vector3_negate(u), eye),
        vector3_dot(vector3_negate(v), eye),
        vector3_dot(vector3_negate(n), eye),
        1.0f,
    };
    
    return result;
}

/**
 * vector4_mul_matrix4:
 * Multiply a four-component vector with a 4x4 matrix
 */

VECTOR4 stVector4D vector4_mul_matrix4(const stVector4D v, const stMatrix4D m)
{
    stVector4D result;
    result.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12] * v.w;
    result.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13] * v.w;
    result.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w;
    result.w = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w;
    
    return result;
}
