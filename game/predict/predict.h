//
//  predict.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#ifndef predict_h
#define predict_h

#include "engine.h"
#include "vector3.h"
#include "array.h"
#include "mesh.h"

#define MODE_NONE       0
#define MODE_ON_GROUND  1
#define MODE_IN_AIR     2

#define RESULT_FLAG_NEEDS_PREREQ  (1 << 0)
#define RESULT_FLAG_FINISHED      (1 << 1)

struct predicted_vector {
    struct Vector3 start;
    struct Vector3 end;
    struct predicted_vector* prev;
    struct predicted_vector* next;
    
    uint64_t time;
    uint64_t time_since_start;
};

struct prediction_param {
    
    int i_mode;
    struct Engine* i_engine;
    struct Actor* i_source;
    struct Actor* i_target_next;
    struct Actor* i_target_final;
    
    struct Vector3 source_position;
    struct Vector3 source_direction;
    
    struct Array* mesh_list;
};

struct route {
    struct predicted_vector* vectors;
    struct predicted_vector* optimal; /* Optimal vector */
    struct route* next;
    
    int flags;
    int mode;
};

struct prediction_param predict_determine_default_param(struct Engine* engine);

const struct Vector3 closest_point_on_vector_segment(struct Vector3 point, struct predicted_vector s);
const float distance_point_to_vector_segment(struct Vector3 point, struct predicted_vector s);

#endif /* predict_h */
