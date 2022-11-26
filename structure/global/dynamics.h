//
//  dynamics.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#ifndef dynamics_h
#define dynamics_h

#include "transform.h"
#include "stream.h"

#define DYNAMICS

#define DYNAMICS_SIZE_BASE      (1 << 0)
#define DYNAMICS_SIZE_ADVANCED  (1 << 1)
#define DYNAMICS_SIZE_COMPLEX   (1 << 2)

struct DynamicsBase {
    
    int32_t object_type;
    uint32_t flags;
    uint32_t flags_end;
    
    float gravity;
    float slopelimit;
    float cos_slope;
    float slide;
    float rebound;
    
    struct Vector3 speed_impose;
    struct Vector3 speed_propose;
    struct Vector3 speed_previous;
    struct Vector3 scale;
    struct Vector3 anim_speed;
    struct Vector3 translation_safe;
    struct Vector3 translation_add;
    
    struct Transform transform_previous;
    struct Transform transform_current;
    struct Matrix3 rotation_imposed;
    
    uint8_t nframe;
    
    address offset;
};

struct DynamicsAdvanced {
    float p_stream;
    float f_stream;
    float previous_slide;
    
    struct Vector3 inertia;
    struct Vector3 f_slide;
    
    struct Vector3 speed_max;
    struct Vector3 speed_stream;
    struct Vector3 speed_add;
    struct Vector3 limit;
    struct Vector3 translation_collide;
    struct Vector3 translation_intertia;
    struct Vector3 ground_normal;
    struct Vector3 wall_normal;
    
    int8_t collide_count;
    
    address offset;
};

struct DynamicsComplex {
    pointer platform_ptr;
    
    float tilt_strength;
    float tilt_inertia;
    float tilt_origin;
    float tilt_angle;
    float hanging_limit;
    
    struct Vector3 contact;
    struct Vector3 translation_fall;
    
    struct SuperObject* platform;
    
    struct Transform matrix_previous_absolute;
    struct Transform matrix_previous_previous;
    
    address offset;
};

struct Dynamics {
    struct DynamicsBase base;
    struct DynamicsAdvanced advanced;
    struct DynamicsComplex complex;

    uint8_t size;
    
    address offset;
};

DYNAMICS struct DynamicsBase dynamics_read_base(struct Stream* stream);
DYNAMICS struct DynamicsAdvanced dynamics_read_advanced(struct Stream* stream);
DYNAMICS struct DynamicsComplex dynamics_read_complex(struct Stream* stream);
DYNAMICS struct Dynamics* dynamics_read(const address addr);

#endif /* dynamics_h */
