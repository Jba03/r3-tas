#ifndef transform_h
#define transform_h

#include "structure.h"
#include "matrix4.h"

#define transform_type_uninitialized            0
#define transform_type_identity                 1
#define transform_type_translate                2
#define transform_type_zoom                     3
#define transform_type_scale                    4
#define transform_type_rotation                 5
#define transform_type_rotation_zoom            6
#define transform_type_rotation_scale           7
#define transform_type_rotation_scale_complex   8
#define transform_type_undefined                9

struct transform
{
    readonly uint32_t type;
    //padding(12)
    readonly matrix4 matrix;
    readonly struct vector4 scale;
};

static inline const struct transform transform_identity()
{
    return (struct transform){ transform_type_identity, matrix4_identity, vector4_new(1.0f, 1.0f, 1.0f, 1.0f) };
}

#endif /* transform_h */
