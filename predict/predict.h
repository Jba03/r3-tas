//
//  predict.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#ifndef predict_h
#define predict_h

#include "vector3.h"

struct LineSegment {
    struct Vector3 start;
    struct Vector3 end;
    struct LineSegment* prev;
    struct LineSegment* next;
};

const struct Vector3 closest_point_on_segment(struct Vector3 point, struct LineSegment S);
const float distance_point_to_segment(struct Vector3 point, struct LineSegment S);

struct PredictedVectorSet {
    
};

#endif /* predict_h */
