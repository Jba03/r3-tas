#ifndef stDynamics_h
#define stDynamics_h

#include "structure.h"
#include "stTransform.h"
#include "stVector3D.h"
#include "stMatrix3D.h"

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
    readonly tdstVector3D axis;
};

struct macdpid
{
    readonly float32 data0;
    readonly tdstVector3D data1;
    readonly tdstVector3D data2;
    readonly tdstVector3D data3;
    readonly float32 data4;
    readonly float32 data5;
    readonly float32 data6;
    readonly struct rotation data7;
    readonly struct rotation data8;
    readonly int8 data9;
    readonly uint16 data10;
    readonly tdstVector3D data11;
    readonly float32 data12;
    readonly tdstVector3D data13;
    readonly float32 data14;
    readonly uint8 data15;
};

struct stDynamicsBaseBlock {
    readonly int32 object_type;
    readonly pointer idcard;
    readonly uint32 flags;
    readonly uint32 endflags;
    readonly float32 gravity;
    readonly float32 slopelimit;
    readonly float32 slope_cosine;
    readonly float32 slide;
    readonly float32 rebound;
    readonly tdstVector3D speed_impose;
    readonly tdstVector3D speed_propose;
    readonly tdstVector3D speed_previous;
    readonly tdstVector3D scale;
    readonly tdstVector3D anim_speed;
    readonly tdstVector3D translation_safe;
    readonly tdstVector3D translation_add;
    //padding(8)
    readonly tdstTransform transform_previous;
    readonly tdstTransform transform_current;
    readonly tdstMatrix3D rotation_imposed;
    readonly uint8 nframe;
    padding(3)
    readonly pointer report;
} typedef tdstDynamicsBaseBlock;

struct stDynamicsAdvancedBlock {
    readonly float32 inertia_x;
    readonly float32 inertia_y;
    readonly float32 inertia_z;
    readonly float32 streamprio;
    readonly float32 streamfactor;
    readonly float32 slide_factor_x;
    readonly float32 slide_factor_y;
    readonly float32 slide_factor_z;
    readonly float32 slide_previous;
    readonly tdstVector3D speed_max;
    readonly tdstVector3D speed_stream;
    readonly tdstVector3D speed_add;
    readonly tdstVector3D limit;
    readonly tdstVector3D collision_translation;
    readonly tdstVector3D inertia_translation;
    readonly tdstVector3D ground_normal;
    readonly tdstVector3D wall_normal;
    readonly int8 collide_count;
    padding(3)
} typedef tdstDynamicsAdvancedBlock;

struct stDynamicsComplexBlock {
    readonly float32 tilt_strength;
    readonly float32 tilt_inertia;
    readonly float32 tilt_origin;
    readonly float32 tilt_angle;
    readonly float32 hanging_limit;
    readonly tdstVector3D contact;
    readonly tdstVector3D fall_translation;
    readonly struct macdpid macdpid;
    readonly pointer platform_so;
    padding(4)
    readonly tdstTransform matrix_previous_absolute;
    readonly tdstTransform matrix_previous_previous;
} typedef tdstDynamicsComplexBlock;

struct stDynamicsObstacle {
    readonly float32 rate;
    readonly tdstVector3D normal;
    readonly tdstVector3D contact;
    readonly pointer material;
    readonly pointer collide_material;
    readonly pointer superobject;
} typedef tdstDynamicsObstacle;

struct stDynamicsMovevement {
    readonly tdstVector3D linear;
    readonly struct rotation angular;
} typedef tdstDynamicsMovevement;

struct stDynamicsReport {
    readonly uint32 surface_state_previous;
    readonly uint32 surface_state_current;
    readonly tdstDynamicsObstacle obstacle;
    readonly tdstDynamicsObstacle ground;
    readonly tdstDynamicsObstacle wall;
    readonly tdstDynamicsObstacle character;
    readonly tdstDynamicsObstacle water;
    readonly tdstDynamicsObstacle ceiling;
    readonly tdstDynamicsMovevement speed_absolute_previous;
    readonly tdstDynamicsMovevement speed_absolute_current;
    readonly tdstDynamicsMovevement position_absolute_previous;
    readonly tdstDynamicsMovevement position_absolute_current;
    readonly char8 bitfield;
    padding(3)
} typedef tdstDynamicsReport;

struct stDynamics {
    readonly tdstDynamicsBaseBlock base;
    readonly tdstDynamicsAdvancedBlock advanced;
    readonly tdstDynamicsComplexBlock complex;
} typedef tdstDynamics;

struct stDynam {
    readonly pointer dynamics;
    readonly pointer parsedata;
    readonly uint32 used_mechanics;
} typedef tdstDynam;

#define dynamics_size(dynam) (host_byteorder_32((dynam).base.endflags) & 0x3)

#if USE_FUNCTIONS

/** dynamics_get_speed: get the speed of the specified dynamics structure */
const struct stVector3D dynamics_get_speed(const tdstDynamics* dynamics);

/** dynamics_collide_with: get the surface collision state for the specified obstacle type */
bool dynamics_collide_with(const tdstDynamics* dynamics, unsigned obstacle);

#endif

#endif /* stDynamics_h */
