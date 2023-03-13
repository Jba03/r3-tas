#ifndef dynamics_h
#define dynamics_h

#include "structure.h"
#include "transform.h"
//#include "vector3.h"
#include "matrix3.h"

/* Base dynamics flags */
#define dynamics_flag_animation                     (1 <<  0) // Use animation speed?
#define dynamics_flag_collide                       (1 <<  1) // Geometry collision enabled?
#define dynamics_flag_gravity                       (1 <<  2) // Is gravity used?
#define dynamics_flag_tilt                          (1 <<  3) // ?
#define dynamics_flag_unknown                       (1 <<  4) // ?
#define dynamics_flag_grounded                      (1 <<  5) // Is on ground
#define dynamics_flag_climb                         (1 <<  6) // Climbing
#define dynamics_flag_collide_dynamics_param        (1 <<  7) // Use dynamics param when colliding?
#define dynamics_flag_collide_preserve_z_momentum   (1 <<  8) // Preseve Z-axis momentum when collding with a wall
#define dynamics_flag_speed_limit                   (1 <<  9) // Limit speed
#define dynamics_flag_inertia                       (1 << 10) // Has inertia?
#define dynamics_flag_stream                        (1 << 11) // Is affected by a stream?
#define dynamics_flag_stuck_to_platform             (1 << 12) // No slide on platform
#define dynamics_flag_has_scale                     (1 << 13) // Use scale parameters
#define dynamics_flag_speed_impose_absolute         (1 << 14) // Impose absolute speed
#define dynamics_flag_speed_propose_absolute        (1 << 15) // Propose absolute speed
#define dynamics_flag_speed_add_absolute            (1 << 16) // Add absolute speed
#define dynamics_flag_speed_impose_x                (1 << 17) // Impose absolute X-speed
#define dynamics_flag_speed_impose_y                (1 << 18) // Impose absolute Y-speed
#define dynamics_flag_speed_impose_z                (1 << 19) // Impose absolute Z-speed
#define dynamics_flag_speed_propose_x               (1 << 20) // Propose absolute X-speed
#define dynamics_flag_speed_propose_y               (1 << 21) // Propose absolute Y-speed
#define dynamics_flag_speed_propose_z               (1 << 22) // Propose absolute Z-speed
#define dynamics_flag_speed_add_x                   (1 << 23) // Add absolute X-speed
#define dynamics_flag_speed_add_y                   (1 << 24) // Add absolute Y-speed
#define dynamics_flag_speed_add_z                   (1 << 25) // Add absolute Z-speed
#define dynamics_flag_limit_x                       (1 << 26) // Limit X (position?)
#define dynamics_flag_limit_y                       (1 << 27) // Limit Y (position?)
#define dynamics_flag_limit_z                       (1 << 28) // Limit Z (position?)
#define dynamics_flag_rotation_impose               (1 << 29) // Impose axis rotation
#define dynamics_flag_platform_lock                 (1 << 30) // ?
#define dynamics_flag_translation_impose            (1 << 31) // Impose translation

/* Base dynamics endflags */
#define dynamics_size_base              (1 << 0)
#define dynamics_size_advanced          (1 << 1)
#define dynamics_size_complex           (1 << 2)
#define dynamics_reserved               (1 << 3)
#define dynamics_mechanics_changed      (1 << 4)

/* Dynamics obstacle surface state */
#define dynamics_obstacle_none      (0 << 0)
#define dynamics_obstacle_ground    (1 << 0)
#define dynamics_obstacle_wall      (1 << 2)
#define dynamics_obstacle_ceiling   (1 << 4)
#define dynamics_obstacle_water     (1 << 6)
#define dynamics_obstacle_error     (1 << 31)

struct rotation
{
    readonly float32 angle;
    readonly struct vector3 axis;
};

struct macdpid
{
    readonly float32 data0;
    readonly struct vector3 data1;
    readonly struct vector3 data2;
    readonly struct vector3 data3;
    readonly float32 data4;
    readonly float32 data5;
    readonly float32 data6;
    readonly struct rotation data7;
    readonly struct rotation data8;
    readonly int8 data9;
    readonly uint16 data10;
    readonly struct vector3 data11;
    readonly float32 data12;
    readonly struct vector3 data13;
    readonly float32 data14;
    readonly uint8 data15;
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
    readonly struct dynamics_complex complex;
};

struct dynam
{
    readonly pointer dynamics;
    readonly pointer parsedata;
    readonly uint32 used_mechanics;
};

#define dynamics_size(dynam) (host_byteorder_32((dynam).base.endflags) & 0x3)

#if USE_FUNCTIONS

/** dynamics_get_speed: get the speed of the specified dynamics structure */
const struct vector3 dynamics_get_speed(const struct dynamics* dynamics);

/** dynamics_collide_with: get the surface collision state for the specified obstacle type */
bool dynamics_collide_with(const struct dynamics* dynamics, unsigned obstacle);

#endif

#endif /* dynamics_h */
