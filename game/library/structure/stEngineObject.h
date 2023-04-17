#ifndef stEngineObject_h
#define stEngineObject_h

#include "structure.h"
#include "stSuperobject.h"

#define actor_family_name   0
#define actor_model_name    1
#define actor_instance_name 2

typedef struct stEngineObject tdstEngineObject, tdstActor;

struct stEngineObject
{
    readonly pointer p3DData;
    readonly pointer stdGame;
    readonly pointer dynam;
    readonly pointer brain;
    readonly pointer cineInfo;
    readonly pointer collSet;
    readonly pointer msWay;
    readonly pointer msLight;
    readonly pointer sectorInfo;
    readonly pointer micro;
    readonly pointer msSound;
};


#ifdef USE_FUNCTIONS

#include "stMatrix4D.h"
#include "stDynamics.h"
#include "stObjectType.h"

/** fnActorGetName: return the family, model, or instance name of specified actor. Null on failure. */
const char* fnActorGetName(int name, const tdstEngineObject* actor, const tdstObjectType* objectType);

/** fnActorGetMatrix: get the global transformation matrix of an actor */
tdstMatrix4D fnActorGetMatrix(const tdstEngineObject* actor);

/** actor_position: get global position of an actor */
tdstVector3D fnActorGetPosition(const tdstEngineObject* actor);

/** actor_speed: get current speed of an actor */
tdstVector3D fnActorGetSpeed(const tdstEngineObject* actor);

/** fnActorGetDynamics: get dynamics structure of an actor */
tdstDynamics* fnActorGetDynamics(const tdstEngineObject* actor);

/** fnActorGetDynamics_report: get dynamics report of an actor */
tdstDynamicsReport* fnActorGetDynamicsReport(const tdstEngineObject* actor);

/**
 * actor_sighting_relative:
 *  compute the horizontal angle between actor A's line of sight and the position of actor B.
 *  The angle returned is in radians, -π to π.
 */
const float fnActorHorizontalSightingRelative(const tdstEngineObject* A, const tdstEngineObject* B);

/**
 * actor_vertical_sighting_relative:
 *  compute the vertical angle between actor A's line of sight and the position of actor B.
 *  The angle returned is in radians, -π to π.
 */
const float fnActorVerticalSightingRelative(const tdstEngineObject* A, const tdstEngineObject* B);

/**
 * actor_trajectory_angle_relative:
 *  compute the angle of actor A's speed relative to the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float fnActorTrajectoryAngleRelative(const tdstEngineObject* A, const tdstEngineObject* B);

void* fnActorGetDsgVar(const tdstEngineObject *object, uint8 var, uint8 *type);

/** fnActorGetCurrentBehaviorName: return the name of the actor's current behavior */
const char* fnActorGetCurrentBehaviorName(const tdstEngineObject* actor);

/** actor_in_behavior: return true if an actor is in the specified behavior */
bool fnActorIsInBehavior(const tdstEngineObject* actor, const char* behavior_name);

tdstEngineObject *fnFindActor(int nameType, const char* name, tdstSuperObject* root);

#endif

#define actor_3Ddata(actor) ((void*)pointer(actor->p3DData))
#define actor_stdgame(actor) ((tdstStandardGameInfo*)pointer(actor->stdGame))
#define actor_dynam(actor) ((tdstDynam*)pointer(actor->dynam))
#define actor_brain(actor) ((tdstBrain*)pointer(actor->brain))
#define actor_cineinfo(actor) ((struct cineinfo*)pointer(actor->cineInfo))
#define actor_collset(actor) ((tdstCollideSet*)pointer(actor->collSet))
#define actor_msway(actor) ((struct msway*)pointer(actor->msWay))
#define actor_mslight(actor) ((struct mslight*)pointer(actor->msLight))
#define actor_sectorinfo(actor) ((struct sectorinfo*)pointer(actor->sectorInfo))
#define actor_micro(actor) ((struct micro*)pointer(actor->micro))
#define actor_mssound(actor) ((struct mssound*)pointer(actor->msSound))

/* Object type */
#define actor_family_type(actor) (actor_stdgame(actor) ? stdgame_family_type(actor_stdgame(actor)) : -1)
#define actor_model_type(actor) (actor_stdgame(actor) ? stdgame_model_type(actor_stdgame(actor)) : -1)
#define actor_instance_type(actor) (actor_stdgame(actor) ? stdgame_instance_type(actor_stdgame(actor)) : -1)

/**
 * actor_superobject:
 *  Get the superobject associated with this actor.
 */
#define actor_superobject(actor) (actor ? (actor_stdgame(actor) ? stdgame_get_superobject(actor_stdgame(actor)) : NULL) : NULL)

#endif /* stEngineObject_h */
