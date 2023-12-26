#ifndef stEngineObject_h
#define stEngineObject_h

//#include "library.hh"
//#include "stSuperObject.hh"
#include "stObjectType.hh"

#include "stDynamics.hh"
#include "stStandardGameInfo.hh"
#include "stBrain.hh"
#include "stCollideSet.hh"

#include <string>

#define actor_family_name   0
#define actor_model_name    1
#define actor_instance_name 2

typedef struct stEngineObject stEngineObject, stActor;

struct stEngineObject {
    readonly pointer<> p3DData;
    readonly pointer<stStandardGameInfo> stdGame;
    readonly pointer<stDynam> dynam;
    readonly pointer<stBrain> brain;
    readonly pointer<uint32_t> cineInfo;
    readonly pointer<stPhysicalCollideSet> collSet;
    readonly pointer<> msWay;
    readonly pointer<> msLight;
    readonly pointer<> sectorInfo;
    readonly pointer<> micro;
    readonly pointer<> msSound;
    
    std::string getName(int type, stObjectType *objectType) {
        int32 ii[] = { stdGame->familyType, stdGame->modelType, stdGame->instanceType };
        std::string name = objectType->getName(type, ii[type]);
        if (type == actor_instance_name && name.length() == 0) {
            name = objectType->getName(actor_model_name, stdGame->modelType) + " (spawned object)";
            if (name.length() == 0)
                name = objectType->getName(actor_family_name, stdGame->familyType);
        }
        return name;
    }
};


#ifdef USE_FUNCTIONS

#include "stMatrix4D.hh"
//#include "stDynamics.hh"
#include "stObjectType.hh"
#include "stStandardGameInfo.hh"

/** fnActorGetName: return the family, model, or instance name of specified actor. Null on failure. */
const char* fnActorGetName(int name, const stEngineObject* actor, const stObjectType* objectType);

/** fnActorGetMatrix: get the global transformation matrix of an actor */
stMatrix4D fnActorGetMatrix(const stEngineObject* actor);

/** actor_position: get global position of an actor */
stVector3D fnActorGetPosition(const stEngineObject* actor);

/** actor_speed: get current speed of an actor */
stVector3D fnActorGetSpeed(const stEngineObject* actor);

/** fnActorGetDynamics: get dynamics structure of an actor */
stDynamics* fnActorGetDynamics(const stEngineObject* actor);

/** fnActorGetDynamics_report: get dynamics report of an actor */
stDynamicsReport* fnActorGetDynamicsReport(const stEngineObject* actor);

/**
 * actor_sighting_relative:
 *  compute the horizontal angle between actor A's line of sight and position P.
 *  The angle returned is in radians, -π to π.
 */
const float fnActorHorizontalSightingRelative(const stEngineObject* A, const stVector3D P);

/**
 * actor_vertical_sighting_relative:
 *  compute the vertical angle between actor A's line of sight and the position of actor B.
 *  The angle returned is in radians, -π to π.
 */
const float fnActorVerticalSightingRelative(const stEngineObject* A, const stEngineObject* B);

/**
 * actor_trajectory_angle_relative:
 *  compute the angle of actor A's speed relative to the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float fnActorTrajectoryAngleRelative(const stEngineObject* A, const stEngineObject* B);

stStandardGameInfo *fnActorGetStdGame(const stEngineObject *object);

void* fnActorGetDsgVar(const stEngineObject *object, uint8 var, uint8 *type);

/** fnActorGetCurrentBehaviorName: return the name of the actor's current behavior */
const char* fnActorGetCurrentBehaviorName(const stEngineObject* actor);

/** actor_in_behavior: return true if an actor is in the specified behavior */
bool fnActorIsInBehavior(const stEngineObject* actor, const char* behavior_name);

stEngineObject *fnFindActor(int nameType, const char* name, stSuperObject* root);

#endif

#define actor_3Ddata(actor) ((void*)pointer(actor->p3DData))
#define actor_stdgame(actor) ((stStandardGameInfo*)pointer(actor->stdGame))
#define actor_dynam(actor) ((stDynam*)pointer(actor->dynam))
#define actor_brain(actor) ((stBrain*)pointer(actor->brain))
#define actor_cineinfo(actor) ((struct cineinfo*)pointer(actor->cineInfo))
#define actor_collset(actor) ((stCollideSet*)pointer(actor->collSet))
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
