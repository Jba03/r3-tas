//
//  dynamics.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#include "dynamics.h"

DYNAMICS struct DynamicsBase dynamics_read_base(struct Stream* stream)
{
    struct DynamicsBase base;
    base.offset = stream->position;
    
    base.object_type = read32S();
    advance(0x4);
    base.flags = read32();
    base.flags_end = read32();
    
    base.gravity = readfloat();
    base.slopelimit = readfloat();
    base.cos_slope = readfloat();
    base.slide = readfloat();
    base.rebound = readfloat();
    
    base.speed_impose       = vector3_read(stream->position += 0 * 3 * 4);
    base.speed_propose      = vector3_read(stream->position += 1 * 3 * 4);
    base.speed_previous     = vector3_read(stream->position += 2 * 3 * 4);
    base.scale              = vector3_read(stream->position += 3 * 3 * 4);
    base.anim_speed         = vector3_read(stream->position += 4 * 3 * 4);
    base.translation_safe   = vector3_read(stream->position += 5 * 3 * 4);
    base.translation_add    = vector3_read(stream->position += 6 * 3 * 4);
    
    advance(0x8); /* padding */
    
    base.transform_previous = transform_read(stream->position += 0 * TRANSFORM_SIZE);
    base.transform_current = transform_read(stream->position += 1 * TRANSFORM_SIZE);
    base.rotation_imposed = matrix3_read(stream->position);
    stream->position += 4 * 3 * 3;
    
    base.nframe = read8();
    advance(11); /* 3, report (4), 4 */
    
    return base;
}

DYNAMICS struct DynamicsAdvanced dynamics_read_advanced(struct Stream* stream)
{
    struct DynamicsAdvanced advanced;
    advanced.offset = stream->position;
    
    advanced.inertia.x = readfloat();
    advanced.inertia.y = readfloat();
    advanced.inertia.z = readfloat();
    
    advanced.p_stream = readfloat();
    advanced.f_stream = readfloat();
    
    /* Slide factor */
    advanced.f_slide.x = readfloat();
    advanced.f_slide.y = readfloat();
    advanced.f_slide.z = readfloat();
    advanced.previous_slide = readfloat();
    
    advanced.speed_max              = vector3_read(stream->position += 0 * 3 * 4);
    advanced.speed_stream           = vector3_read(stream->position += 1 * 3 * 4);
    advanced.speed_add              = vector3_read(stream->position += 2 * 3 * 4);
    advanced.limit                  = vector3_read(stream->position += 3 * 3 * 4);
    advanced.translation_collide    = vector3_read(stream->position += 4 * 3 * 4);
    advanced.translation_intertia   = vector3_read(stream->position += 5 * 3 * 4);
    advanced.ground_normal          = vector3_read(stream->position += 6 * 3 * 4);
    advanced.wall_normal            = vector3_read(stream->position += 7 * 3 * 4);
    
    advanced.collide_count = read8S();
    
    return advanced;
}

DYNAMICS struct DynamicsComplex dynamics_read_complex(struct Stream* stream)
{
    struct DynamicsComplex complex;
    complex.offset = stream->position;
    
    return complex;
}

DYNAMICS struct Dynamics* dynamics_read(const address addr)
{
    struct Dynamics* dynamics = malloc(sizeof *dynamics);
    dynamics->offset = addr;
    
    struct Stream* stream = stream_open(addr);
    dynamics->base = dynamics_read_base(stream);
    
    dynamics->size = dynamics->base.flags_end & 0x3;
    if (dynamics->size & DYNAMICS_SIZE_ADVANCED) dynamics->advanced = dynamics_read_advanced(stream);
    if (dynamics->size & DYNAMICS_SIZE_COMPLEX) dynamics->complex = dynamics_read_complex(stream);
    
    stream_close(stream);
    
    return dynamics;
}
