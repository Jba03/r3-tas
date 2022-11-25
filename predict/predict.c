//
//  predict.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#include "predict.h"
#include "common.h"

const struct Vector3 closest_point_on_segment(struct Vector3 point, struct LineSegment s)
{
    const struct Vector3 L = vector3_sub(s.end, s.start);
    const struct Vector3 direction = vector3_normalize(L);
    const struct Vector3 v = vector3_sub(point, s.start);
    
    float d = vector3_dot(v, direction);
    d = min(max(0.0f, d), vector3_length(L));
    
    return vector3_add(s.start, vector3_mulf(direction, d));
}

const float distance_point_to_segment(struct Vector3 point, struct LineSegment S)
{
    struct Vector3 p = closest_point_on_segment(point, S);
    struct Vector3 d = vector3_sub(point, p);
    return vector3_length(d);
}
