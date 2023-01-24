#ifndef transform_h
#define transform_h

#include "structure.h"
#include "matrix4.h"

#define transform_uninitialized             0
#define transform_identity                  1
#define transform_translate                 2
#define transform_zoom                      3
#define transform_scale                     4
#define transform_rotation                  5
#define transform_rotation_zoom             6
#define transform_rotation_scale            7
#define transform_rotation_scale_complex    8
#define transform_undefined                 9

struct transform
{
    readonly uint32_t type;
    padding(12)
    readonly matrix4 matrix;
    readonly struct vector3 scale;
};

#endif /* transform_h */
