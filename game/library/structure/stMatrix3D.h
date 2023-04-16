#ifndef stMatrix3D_h
#define stMatrix3D_h

#include "structure.h"

typedef union stMatrix3D tdstMatrix3D;

union stMatrix3D
{
    struct
    {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
    };
    
    struct
    {
        float m[9];
    };
};

static const union stMatrix3D matrix3_identity =
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

#endif /* stMatrix3D_h */
