#ifndef stDynamics_h
#define stDynamics_h

#include "stTransform.hh"
#include "stVector3D.hh"
#include "stMatrix3D.hh"
#include "stSuperObject.hh"

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

typedef struct stDynamicsBaseBlock stDynamicsBaseBlock;
typedef struct stDynamicsAdvancedBlock stDynamicsAdvancedBlock;
typedef struct stDynamicsComplexBlock stDynamicsComplexBlock;
typedef struct stDynamicsObstacle stDynamicsObstacle;
typedef struct stDynamicsMovevement stDynamicsMovevement;
typedef struct stDynamicsReport stDynamicsReport;
typedef struct stDynamics stDynamics;
typedef struct stDynam stDynam;

struct rotation {
    readonly float32 angle;
    readonly stVector3D axis;
};

struct macdpid {
    readonly float32 data0;
    readonly stVector3D data1;
    readonly stVector3D data2;
    readonly stVector3D data3;
    readonly float32 data4;
    readonly float32 data5;
    readonly float32 data6;
    readonly rotation data7;
    readonly rotation data8;
    readonly int8 data9;
    readonly uint16 data10;
    readonly stVector3D data11;
    readonly float32 data12;
    readonly stVector3D data13;
    readonly float32 data14;
    readonly uint8 data15;
};

struct stDynamicsBaseBlock {
    readonly int32 objectType;
    readonly pointer<> idcard;
    readonly uint32 flags;
    readonly uint32 endFlags;
    readonly float32 gravity;
    readonly float32 slopeLimit;
    readonly float32 slopeCosine;
    readonly float32 slide;
    readonly float32 rebound;
    readonly stVector3D imposedSpeed;
    readonly stVector3D proposedSpeed;
    readonly stVector3D previousSpeed;
    readonly stVector3D scale;
    readonly stVector3D animationSpeed;
    readonly stVector3D safeTranslation;
    readonly stVector3D addedTranslation;
    readonly stTransform previousTransform;
    readonly stTransform currentTransform;
    readonly stMatrix3D imposedRotation;
    readonly uint8 nFrame;
    readonly padding(3)
    readonly pointer<> report;
};

struct stDynamicsAdvancedBlock {
    readonly float32 xInertia;
    readonly float32 yInertia;
    readonly float32 zInertia;
    readonly float32 streamPriority;
    readonly float32 streamFactor;
    readonly float32 xSlideFactor;
    readonly float32 ySlideFactor;
    readonly float32 zSlideFactor;
    readonly float32 previousSlide;
    readonly stVector3D maxSpeed;
    readonly stVector3D streamSpeed;
    readonly stVector3D addedSpeed;
    readonly stVector3D limit;
    readonly stVector3D collisionTranslation;
    readonly stVector3D inertiaTranslation;
    readonly stVector3D groundNormal;
    readonly stVector3D wallNormal;
    readonly int8 collideCount;
    readonly padding(3)
};

struct stDynamicsComplexBlock {
    readonly float32 tiltStrength;
    readonly float32 tiltInertia;
    readonly float32 tiltOrigin;
    readonly float32 tiltAngle;
    readonly float32 hangingLimit;
    readonly stVector3D contact;
    readonly stVector3D fallTranslation;
    readonly macdpid macdpid;
    readonly pointer<> platformSuperObject;
    readonly padding(4)
    readonly stTransform previousMatrixAbsolute;
    readonly stTransform previousMatrixPrevious;
};

struct stDynamicsObstacle
{
    readonly float32 rate;
    readonly stVector3D normal;
    readonly stVector3D contact;
    readonly pointer<> material;
    readonly pointer<> collideMaterial;
    readonly pointer<> superObject;
};

struct stDynamicsMovevement
{
    readonly stVector3D linear;
    readonly rotation angular;
};

struct stDynamicsReport
{
    readonly uint32 previousSurfaceState;
    readonly uint32 currentSurfaceState;
    readonly stDynamicsObstacle obstacle;
    readonly stDynamicsObstacle ground;
    readonly stDynamicsObstacle wall;
    readonly stDynamicsObstacle character;
    readonly stDynamicsObstacle water;
    readonly stDynamicsObstacle ceiling;
    readonly stDynamicsMovevement previousAbsoluteSpeed;
    readonly stDynamicsMovevement currentAbsoluteSpeed;
    readonly stDynamicsMovevement previousAbsolutePosition;
    readonly stDynamicsMovevement currentAbsolutePosition;
    readonly char8 bitField;
    readonly padding(3)
};

struct stDynamics
{
    readonly stDynamicsBaseBlock base;
    readonly stDynamicsAdvancedBlock advanced;
    readonly stDynamicsComplexBlock complex;
};

struct stDynam
{
    readonly pointer<stDynamics> dynamics;
    readonly pointer<> parsData;
    readonly uint32 usedMechanics;
};

#define dynamics_size(dynam) (host_byteorder_32((dynam).base.endflags) & 0x3)

#ifdef USE_FUNCTIONS

/** fnDynamicsGetSpeed: get the speed of the specified dynamics structure */
const struct stVector3D fnDynamicsGetSpeed(const stDynamics* dynamics);

/** fnDynamicsCollideWith: get the surface collision state for the specified obstacle type */
bool fnDynamicsCollideWith(const stDynamics* dynamics, unsigned obstacle);

#endif

#endif /* stDynamics_h */
