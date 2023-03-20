#ifndef stEngineObject_h
#define stEngineObject_h

#include "structure.h"

#define actor_family_name   0
#define actor_model_name    1
#define actor_instance_name 2

struct stEngineObject {
    readonly pointer p_3ddata;
    readonly pointer stdgame;
    readonly pointer dynam;
    readonly pointer brain;
    readonly pointer cineinfo;
    readonly pointer collset;
    readonly pointer msway;
    readonly pointer mslight;
    readonly pointer sectorinfo;
    readonly pointer micro;
    readonly pointer mssound;
} typedef tdstEngineObject, tdstActor;

#if USE_FUNCTIONS

#include "stMatrix4D.h"
#include "stDynamics.h"
#include "stObjectType.h"

/** actor_matrix: get the global transformation matrix of an actor */
const tdstMatrix4D actor_matrix(const tdstEngineObject* actor);

/** actor_position: get global position of an actor */
const tdstVector3D actor_position(const tdstEngineObject* actor);

/** actor_speed: get current speed of an actor */
const tdstVector3D actor_speed(const tdstEngineObject* actor);

/**
 * actor_sighting_relative:
 *  compute the horizontal angle between actor A's line of sight and the position of actor B.
 *  The angle returned is in radians, -π to π.
 */
const float actor_horizontal_sighting_relative(const tdstEngineObject* A, const tdstEngineObject* B);

/**
 * actor_vertical_sighting_relative:
 *  compute the vertical angle between actor A's line of sight and the position of actor B.
 *  The angle returned is in radians, -π to π.
 */
const float actor_vertical_sighting_relative(const tdstEngineObject* A, const tdstEngineObject* B);

/**
 * actor_trajectory_angle_relative:
 *  compute the angle of actor A's speed relative to the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float actor_trajectory_angle_relative(const tdstEngineObject* A, const tdstEngineObject* B);

/** actor_dsgvar: get the offset of an actor's dsgvar. Negative on failure. */
const int actor_dsgvar(const tdstEngineObject* actor, unsigned var, int* type, void** data);

/** actor_name: return the family, model, or instance name of specified actor. Null on failure. */
const char* actor_name(int name, const tdstEngineObject* actor, const tdstObjectType* objectType);

/** actor_current_behavior_name: return the name of the actor's current behavior */
const char* actor_current_behavior_name(const tdstEngineObject* actor);

/** actor_in_behavior: return true if an actor is in the specified behavior */
bool actor_in_behavior(const tdstEngineObject* actor, const char* behavior_name);

/** actor_dynamics: get dynamics structure of an actor */
const tdstDynamics* actor_dynamics(const tdstEngineObject* actor);

/** actor_dynamics_report: get dynamics report of an actor */
const tdstDynamicsReport* actor_dynamics_report(const tdstEngineObject* actor);

#endif

#define actor_3Ddata(actor) ((void*)pointer(actor->p_3ddata))
#define actor_stdgame(actor) ((tdstStandardGameInfo*)pointer(actor->stdgame))
#define actor_dynam(actor) ((tdstDynam*)pointer(actor->dynam))
#define actor_brain(actor) ((tdstBrain*)pointer(actor->brain))
#define actor_cineinfo(actor) ((struct cineinfo*)pointer(actor->cineinfo))
#define actor_collset(actor) ((tdstCollideSet*)pointer(actor->collset))
#define actor_msway(actor) ((struct msway*)pointer(actor->msway))
#define actor_mslight(actor) ((struct mslight*)pointer(actor->mslight))
#define actor_sectorinfo(actor) ((struct sectorinfo*)pointer(actor->sectorinfo))
#define actor_micro(actor) ((struct micro*)pointer(actor->micro))
#define actor_mssound(actor) ((struct mssound*)pointer(actor->mssound))

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
