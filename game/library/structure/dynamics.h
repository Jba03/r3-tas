#ifndef dynamics_h
#define dynamics_h

#include "structure.h"
#include "transform.h"
//#include "vector3.h"
#include "matrix3.h"

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
    readonly int8 data9;
    padding(1)
    readonly uint16 data10;
    readonly struct vector3 data11;
    readonly float32 data12;
    readonly struct vector3 data13;
    readonly float32 data14;
    readonly uint8 data15;
    padding(3)
};

struct dynamics_base
{
    readonly int32 object_type;
    readonly pointer idcard;
    readonly uint32 flags;
    readonly uint32 endflags;
    readonly float32 gravity;
    readonly float32 slopelimit;
    readonly float32 slope_cosine;
    readonly float32 slide;
    readonly float32 rebound;
    readonly struct vector3 speed_impose;
    readonly struct vector3 speed_propose;
    readonly struct vector3 speed_previous;
    readonly struct vector3 scale;
    readonly struct vector3 anim_speed;
    readonly struct vector3 translation_safe;
    readonly struct vector3 translation_add;
    //padding(8)
    readonly struct transform transform_previous;
    readonly struct transform transform_current;
    readonly matrix3 rotation_imposed;
    readonly uint8 nframe;
    padding(3)
    readonly pointer report;
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
    readonly int8 collide_count;
    padding(3)
};

struct dynamics_complex
{
    readonly float32 tilt_strength;
    readonly float32 tilt_inertia;
    readonly float32 tilt_origin;
    readonly float32 tilt_angle;
    readonly float32 hanging_limit;
    readonly struct vector3 contact;
    readonly struct vector3 fall_translation;
    readonly struct macdpid macdpid;
    readonly pointer platform_so;
    padding(4)
    readonly struct transform matrix_previous_absolute;
    readonly struct transform matrix_previous_previous;
};

struct dynamics_obstacle
{
    readonly float32 rate;
    readonly struct vector3 normal;
    readonly struct vector3 contact;
    readonly pointer material;
    readonly pointer collide_material;
    readonly pointer superobject;
};

struct rotation
{
    readonly float32 angle;
    readonly struct vector3 axis;
};

struct movement_vector
{
    readonly struct vector3 linear;
    readonly struct rotation angular;
};

struct dynamics_report
{
    readonly uint32 surface_state_previous;
    readonly uint32 surface_state_current;
    readonly struct dynamics_obstacle obstacle;
    readonly struct dynamics_obstacle ground;
    readonly struct dynamics_obstacle wall;
    readonly struct dynamics_obstacle character;
    readonly struct dynamics_obstacle water;
    readonly struct dynamics_obstacle ceiling;
    readonly struct movement_vector speed_absolute_previous;
    readonly struct movement_vector speed_absolute_current;
    readonly struct movement_vector position_absolute_previous;
    readonly struct movement_vector position_absolute_current;
    readonly char8 bitfield;
    padding(3)
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
    readonly uint32 used_mechanics;
};

#define dynamics_size(dynam) (host_byteorder_32((dynam).base.endflags) & 0x3)

#endif /* dynamics_h */
