#ifndef dynamics_h
#define dynamics_h

#include "structure.h"
#include "transform.h"
#include "vector.h"

#define dynamics_size_base      0x1
#define dynamics_size_advanced  0x2
#define dynamics_size_complex   0x4

struct macdpid
{
    readonly float32 data0;
    readonly struct vector3 data1;
    readonly struct vector3 data2;
    readonly struct vector3 data3;
    readonly float32 data4;
    readonly float32 data5;
    readonly float32 data6;
    readonly struct { readonly float angle; readonly struct vector3 axis; } data7;
    readonly struct { readonly float angle; readonly struct vector3 axis; } data8;
    readonly int8_t data9;
    padding(1)
    readonly uint16_t data10;
    readonly struct vector3 data11;
    readonly float32 data12;
    readonly struct vector3 data13;
    readonly float32 data14;
    readonly uint8_t data15;
    padding(3)
};

struct dynamics_base
{
    readonly int32_t object_type;
    readonly pointer idcard;
    readonly uint32_t flags;
    readonly uint32_t endflags;
    readonly float32 gravity;
    readonly float32 slopelimit;
    readonly float32 slope_cosine;
    readonly float32 slide;
    readonly float32 rebound;
    readonly struct vector33 speed_impose;
    readonly struct vector33 speed_propose;
    readonly struct vector33 speed_previous;
    readonly struct vector33 scale;
    readonly struct vector33 anim_speed;
    readonly struct vector33 translation_safe;
    readonly struct vector33 translation_add;
    padding(8)
    readonly struct transform transform_previous;
    readonly struct transform transform_current;
    readonly struct matrix3x3 rotation_imposed;
    readonly uint8_t nframe;
    padding(3)
    readonly pointer report;
    padding(4)
};

struct dynamics_advanced
{
    readonly float32 inertia_x;
    readonly float32 inertia_y;
    readonly float32 inertia_z;
    readonly float32 streamprio;
    readonly float32 streamfactor;
    readonly float32 slide_factor_x;
    readonly float32 slide_factor_y;
    readonly float32 slide_factor_z;
    readonly float32 slide_previous;
    readonly struct vector3 speed_max;
    readonly struct vector3 speed_stream;
    readonly struct vector3 speed_add;
    readonly struct vector3 limit;
    readonly struct vector3 collision_translation;
    readonly struct vector3 inertia_translation;
    readonly struct vector3 ground_normal;
    readonly struct vector3 wall_normal;
    readonly int8_t collide_count;
    padding(3)
};

struct dynamics_complex
{
    readonly float tilt_strength;
    readonly float tilt_inertia;
    readonly float tilt_origin;
    readonly float tilt_angle;
    readonly float hanging_limit;
    readonly struct vector3 contact;
    readonly struct vector3 fall_translation;
    readonly struct macdpid macdpid;
    readonly pointer platform_so;
    padding(4)
    readonly struct transform matrix_previous_absolute;
    readonly struct transform matrix_previous_previous;
};

struct dynamics
{
    readonly struct dynamics_base base;
    readonly struct dynamics_advanced advanced;
    padding(8)
    readonly struct dynamics_complex complex;
};

struct dynam
{
    readonly pointer dynamics;
    readonly pointer parsedata;
    readonly uint32_t used_mechanics;
};

#define dynamics_size(dynam) (host_byteorder_32((dynam).base.endflags) & 0x3)

#endif /* dynamics_h */
