#ifndef matrix_h
#define matrix_h

struct matrix3x3
{
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
};

struct matrix4x4
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
};

#endif /* matrix_h */
