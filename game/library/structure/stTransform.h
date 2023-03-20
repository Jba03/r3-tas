#ifndef stTransform_h
#define stTransform_h

#include "structure.h"
#include "stMatrix4D.h"

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

struct stTransform {
    readonly uint32_t type;
    //padding(12)
    readonly tdstMatrix4D matrix;
    readonly tdstVector4D scale;
} typedef tdstTransform;

static inline const tdstTransform transform_identity()
{
    return (tdstTransform){ transform_type_identity, matrix4_identity, vector4_new(1.0f, 1.0f, 1.0f, 1.0f) };
}

#endif /* stTransform_h */
