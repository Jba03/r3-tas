/*******************************************************
 * matrix4.c: 4x4 matrix operations
 *******************************************************
 * Created by Jba03 on 2022-10-04
 * Copyright (c) 2021 - 2022 Jba03. All rights reserved.
 *******************************************************/

#include "matrix4.h"
#include "memory.h"

#include <assert.h>
#include <math.h>

#pragma mark - Matrix4

const union Matrix4 matrix4_identity =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

MATRIX4 union Matrix4 matrix4_read(const address addr)
{
    union Matrix4 m = matrix4_identity;
    for (int i = 0; i < 16; i++)
        m.m[i] = memory.read_float(addr + 4 * i);
    return m;
}

MATRIX4 union Matrix4 matrix4_new(const union Vector4 row0, const union Vector4 row1, const union Vector4 row2, const union Vector4 row3)
{
    union Matrix4 m;
    m.row0 = row0;
    m.row1 = row1;
    m.row2 = row2;
    m.row3 = row3;
    
    return m;
}

MATRIX4 union Matrix4 matrix4_mul(const union Matrix4 a, const union Matrix4 b)
{
    union Matrix4 m;
    
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

MATRIX4 union Matrix4 matrix4_inverse(const union Matrix4 m)
{
    union Matrix4 result;
    
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

MATRIX4 union Matrix4 matrix4_transpose(const union Matrix4 m)
{
    union Matrix4 result;
    
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

MATRIX4 union Matrix4 matrix4_make_translation(const float x, const float y, const float z)
{
    union Matrix4 result = matrix4_identity;
    result.m03 = x;
    result.m13 = y;
    result.m23 = z;
    
    return result;
}

MATRIX4 union Matrix4 matrix4_make_rotation_x(const float radians)
{
    float sine = sin(radians);
    float cosine = cos(radians);
    
    union Matrix4 result = matrix4_identity;
    result.m11 = cosine;
    result.m12 = sine;
    result.m21 = -sine;
    result.m22 = cosine;
    
    return result;
}

MATRIX4 union Matrix4 matrix4_make_rotation_y(const float radians)
{
    const float sine = sin(radians);
    const float cosine = cos(radians);
    
    union Matrix4 result = matrix4_identity;
    result.m00 = cosine;
    result.m02 = -sine;
    result.m20 = sine;
    result.m22 = cosine;
    
    return result;
}

MATRIX4 union Matrix4 matrix4_make_rotation_z(const float radians)
{
    const float sine = sin(radians);
    const float cosine = cos(radians);
    
    union Matrix4 result = matrix4_identity;
    result.m00 = cosine;
    result.m01 = sine;
    result.m10 = -sine;
    result.m11 = cosine;
    
    return result;
}

MATRIX4 union Matrix4 matrix4_make_scale(const float x, const float y, const float z)
{
    union Matrix4 result = matrix4_identity;
    result.m00 = x;
    result.m11 = y;
    result.m22 = z;
    
    return result;
}

MATRIX4 union Matrix4 matrix4_make_scale_vector3(const union Vector3 v)
{
    union Matrix4 result =
    {
        v.x, 0.0f, 0.0f, 0.0f,
        0.0f, v.y, 0.0f, 0.0f,
        0.0f, 0.0f, v.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    
    return result;
}

MATRIX4 union Matrix4 matrix4_perspective(const float fov_y, const float aspect, const float near_z, const float far_z)
{
    union Matrix4 result = matrix4_identity;
    
    const float ct = 1.0f / tanf(fov_y / 2.0f);
    
    result.m00 = ct / aspect;
    result.m11 = ct;
    result.m22 = (far_z + near_z) / (near_z - far_z);
    result.m23 = -1.0f;
    result.m32 = (2.0f * far_z * near_z) / (near_z - far_z);
    result.m33 = 0.0f;
    
    return result;
}

MATRIX4 union Matrix4 matrix4_ortho(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    float ral = right + left;
    float rsl = right - left;
    float tab = top + bottom;
    float tsb = top - bottom;
    float fan = far + near;
    float fsn = far - near;
    
    union Matrix4 result =
    {
        2.0f / rsl, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / tsb, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / fsn, 0.0f,
        -ral / rsl, -tab / tsb, -fan / fsn, 1.0f
    };
    
    return result;
}

MATRIX4 union Matrix4 matrix4_lookat(const union Vector3 eye, const union Vector3 center, const union Vector3 up)
{
    union Vector3 n = vector3_normalize(vector3_add(eye, vector3_negate(center)));
    union Vector3 u = vector3_normalize(vector3_cross(up, n));
    union Vector3 v = vector3_cross(n, u);
    
    union Matrix4 result =
    {
        u.v[0], v.v[0], n.v[0], 0.0f,
        u.v[1], v.v[1], n.v[1], 0.0f,
        u.v[2], v.v[2], n.v[2], 0.0f,
        vector3_dot(vector3_negate(u), eye),
        vector3_dot(vector3_negate(v), eye),
        vector3_dot(vector3_negate(n), eye),
        1.0f,
    };
    
    return result;
}

#if defined(VECTOR4)

VECTOR4 union Vector4 vector4_mul_matrix4(const union Vector4 v, const union Matrix4 m)
{
    union Vector4 result;
    result.x = m.m[0] * v.v[0] + m.m[4] * v.v[1] + m.m[8]  * v.v[2] + m.m[12] * v.v[3];
    result.y = m.m[1] * v.v[0] + m.m[5] * v.v[1] + m.m[9]  * v.v[2] + m.m[13] * v.v[3];
    result.z = m.m[2] * v.v[0] + m.m[6] * v.v[1] + m.m[10] * v.v[2] + m.m[14] * v.v[3];
    result.w = m.m[3] * v.v[0] + m.m[7] * v.v[1] + m.m[11] * v.v[2] + m.m[15] * v.v[3];
    
    return result;
}

#endif
