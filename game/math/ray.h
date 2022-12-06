//
//  ray.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#ifndef ray_h
#define ray_h

#include "vector2.h"
#include "vector3.h"
#include "matrix4.h"

#define RAY

struct Ray {
    struct Vector3 origin;
    struct Vector3 direction;
};

RAY const struct Ray raycast(const struct Vector2 coord,
                             const struct Matrix4 projection,
                             const struct Matrix4 view,
                             const float near,
                             const float far);

#endif /* ray_h */
