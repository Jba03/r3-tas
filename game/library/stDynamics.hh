#ifndef stDynamics_h
#define stDynamics_h

#include "stTransform.hh"
#include "stVector3D.hh"
#include "stMatrix3D.hh"
#include "stSuperObject.hh"

/* dynamics->base.flags */
#define dynamicsFlagAnimation                 (1 <<  0) // Use animation speed?
#define dynamicsFlagCollide                   (1 <<  1) // Geometry collision enabled?
#define dynamicsFlagGravity                   (1 <<  2) // Is gravity used?
#define dynamicsFlagTilt                      (1 <<  3) // ?
#define dynamicsFlagUnknown                   (1 <<  4) // ?
#define dynamicsFlagGrounded                  (1 <<  5) // Is on ground
#define dynamicsFlagClimb                     (1 <<  6) // Climbing
#define dynamicsFlagCollideDynamicsParam      (1 <<  7) // Use dynamics param when colliding?
#define dynamicsFlagCollidePreserveZMomentum  (1 <<  8) // Preseve Z-axis momentum when colliding with a wall
#define dynamicsFlagSpeedLimit                (1 <<  9) // Limit speed
#define dynamicsFlagInertia                   (1 << 10) // Has inertia?
#define dynamicsFlagStream                    (1 << 11) // Is affected by a stream?
#define dynamicsFlagStuckToPlatform           (1 << 12) // No slide on platform
#define dynamicsFlagHasScale                  (1 << 13) // Use scale parameters
#define dynamicsFlagSpeedImposeAbsolute       (1 << 14) // Impose absolute speed
#define dynamicsFlagSpeedProposeAbsolute      (1 << 15) // Propose absolute speed
#define dynamicsFlagSpeedAddAbsolute          (1 << 16) // Add absolute speed
#define dynamicsFlagSpeedImposeX              (1 << 17) // Impose absolute X-speed
#define dynamicsFlagSpeedImposeY              (1 << 18) // Impose absolute Y-speed
#define dynamicsFlagSpeedImposeZ              (1 << 19) // Impose absolute Z-speed
#define dynamicsFlagSpeedProposeX             (1 << 20) // Propose absolute X-speed
#define dynamicsFlagSpeedProposeY             (1 << 21) // Propose absolute Y-speed
#define dynamicsFlagSpeedProposeZ             (1 << 22) // Propose absolute Z-speed
#define dynamicsFlagSpeedAddX                 (1 << 23) // Add absolute X-speed
#define dynamicsFlagSpeedAddY                 (1 << 24) // Add absolute Y-speed
#define dynamicsFlagSpeedAddZ                 (1 << 25) // Add absolute Z-speed
#define dynamicsFlagLimitX                    (1 << 26) // Limit X (position?)
#define dynamicsFlagLimitY                    (1 << 27) // Limit Y (position?)
#define dynamicsFlagLimitZ                    (1 << 28) // Limit Z (position?)
#define dynamicsFlagRotationImpose            (1 << 29) // Impose axis rotation
#define dynamicsFlagPlatformLock              (1 << 30) // ?
#define dynamicsFlagTranslationImpose         (1 << 31) // Impose translation

/* dynamics->base.endFlags */
#define dynamicsSizeBase              (1 << 0)
#define dynamicsSizeAdvanced          (1 << 1)
#define dynamicsSizeComplex           (1 << 2)
#define dynamicsReserved              (1 << 3)
#define dynamicsMechanicsChanged      (1 << 4)

/* Dynamics obstacle surface state */
#define dynamics_obstacle_none      (0 << 0)
#define dynamics_obstacle_ground    (1 << 0)
#define dynamics_obstacle_wall      (1 << 2)
#define dynamics_obstacle_ceiling   (1 << 4)
#define dynamics_obstacle_water     (1 << 6)
#define dynamics_obstacle_error     (1 << 31)

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
  readonly stVector3D imposeSpeed;
  readonly stVector3D proposeSpeed;
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

struct stDynamicsObstacle {
  readonly float32 rate;
  readonly stVector3D normal;
  readonly stVector3D contact;
  readonly pointer<> material;
  readonly pointer<> collideMaterial;
  readonly pointer<> superObject;
};

struct stDynamicsMovevement {
  readonly stVector3D linear;
  readonly rotation angular;
};

struct stDynamicsReport {
  readonly uint32 previousSurfaceState = 0;
  readonly uint32 currentSurfaceState = 0;
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

struct stDynamics {
  
  enum size {
    BASE,
    ADVANCED,
    COMPLEX,
  };
    
    readonly stDynamicsBaseBlock base;
    readonly stDynamicsAdvancedBlock advanced;
    readonly stDynamicsComplexBlock complex;
    
    enum size size()
    {
        switch (base.endFlags & 3)
        {
            case 0: return BASE;
            case 1: return ADVANCED;
            case 2: return COMPLEX;
            default: return COMPLEX;
        }
    }
};

struct stDynam
{
    readonly pointer<stDynamics> dynamics;
    readonly pointer<> parseData;
    readonly uint32 usedMechanics;
};

#endif /* stDynamics_h */
