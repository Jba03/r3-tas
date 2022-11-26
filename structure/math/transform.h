//
//  transform.h
//  r3tools
//
//  Created by Jba03 on 2022-11-14.
//

#ifndef transform_h
#define transform_h

#include "matrix3.h"
#include "matrix4.h"

#define TRANSFORM

enum TransformType {
    TRANSFORM_UNINITIALIZED = 0,
    TRANSFORM_IDENTITY = 1,
    TRANSFORM_TRANSLATE = 2,
    TRANSFORM_ZOOM = 3,
    TRANSFORM_SCALE = 4,
    TRANSFORM_ROTATION = 5,
    TRANSFORM_ROTATION_ZOOM = 6,
    TRANSFORM_ROTATION_SCALE = 7,
    TRANSFORM_ROTATION_SCALE_COMPLEX = 8,
    TRANSFORM_UNDEFINED = 9,
};

struct Transform {
#define TRANSFORM_SIZE 0x60
    
    struct Matrix4 matrix;
    struct Vector4 scale;
    enum TransformType type;
    
    address offset;
};

TRANSFORM struct Transform transform_read(const address addr);

#endif /* transform_h */
