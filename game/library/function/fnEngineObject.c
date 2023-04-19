//
//  actor.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-19.
//

#include <stdio.h>

#include "game.h"
#include "memory.h"

#include "stSuperobject.h"
#include "stEngineObject.h"
#include "stBrain.h"
#include "stMind.h"
#include "stIntelligence.h"
#include "stBehavior.h"
#include "stStandardGameInfo.h"
#include "stDynamics.h"
#include "stDsg.h"
#include "stObjectType.h"

#include "fnDsg.c"

tdstDynamics* fnActorGetDynamics(const tdstEngineObject* actor)
{
    const tdstDynam* dynam = pointer(actor->dynam);
    return dynam ? pointer(dynam->dynamics) : NULL;
}

tdstDynamicsReport* fnActorGetDynamicsReport(const tdstEngineObject* actor)
{
    const tdstDynamics* dynamics = fnActorGetDynamics(actor);
    return dynamics ? pointer(dynamics->base.report) : NULL;
}

tdstMatrix4D fnActorGetMatrix(const tdstEngineObject* actor)
{
    if (!actor) return matrix4_identity;
    const tdstSuperObject* so = actor_superobject(actor);
    return fnSuperobjectGetGlobalMatrix(so);
}

tdstVector3D fnActorGetPosition(const tdstEngineObject* actor)
{
    const tdstMatrix4D T = fnActorGetMatrix(actor);
    return game_matrix4_position(T);
}

tdstVector3D fnActorGetSpeed(const tdstEngineObject* actor)
{
    const tdstDynamics* dynamics = fnActorGetDynamics(actor);
    return fnDynamicsGetSpeed(dynamics);
}

/**
 * fnActorRelativeHorizontalSighting:
 *  compute the horizontal angle between actor A's line of sight and the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float fnActorHorizontalSightingRelative(const tdstEngineObject* A, const tdstEngineObject* B)
{
    const tdstMatrix4D mA = fnActorGetMatrix(A);
    const tdstMatrix4D mB = fnActorGetMatrix(B);

    const tdstVector3D pA = game_matrix4_position(mA);
    const tdstVector3D pB = game_matrix4_position(mB);
    
    // Calculate the world coordinate space
    // angle to `B`from the perspective of `A`
    const float hR = atan2(pB.y - pA.y, pB.x - pA.x);
    // Calculate the look angle of `A`
    const float lA = atan2(mA.m00, mA.m01);
    // Then, calculate the relative angle of both
    float angle = hR + lA;
    
    if (angle <= -M_PI) angle =  (2.0f * M_PI + angle);
    if (angle >=  M_PI) angle = -(2.0f * M_PI - angle);
    return angle;
}

/**
 * fnActorVerticalSightingRelative:
 *  compute the vertical angle between actor A's line of sight and the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float fnActorVerticalSightingRelative(const tdstEngineObject* A, const tdstEngineObject* B)
{
    const tdstMatrix4D mA = fnActorGetMatrix(A);
    const tdstMatrix4D mB = fnActorGetMatrix(B);

    const tdstVector3D pA = game_matrix4_position(mA);
    const tdstVector3D pB = game_matrix4_position(mB);
    
    // Calculate the distance between the two objects
    const float l = vector3_length(vector3_sub(pB, pA));
    // The cathetus becomes the difference in height
    const float h = pB.z - pA.z;
    // arcsin(h / l)
    const float v = asin(h / l);
    // TODO: Use better matrix rotation values
    const float lA = -atan2(mA.m12, mA.m22);
    
    float angle = lA - v;
    if (angle <= -M_PI) angle =  (2.0f * M_PI + angle);
    if (angle >=  M_PI) angle = -(2.0f * M_PI - angle);
    return angle;
}

/**
 * actor_trajectory_angle_relative:
 *  compute the angle of actor A's speed relative to the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float fnActorTrajectoryAngleRelative(const tdstEngineObject* A, const tdstEngineObject* B)
{
    if (!A || !B) return M_PI;
    
    const tdstDynam* dynam = actor_dynam(A);
    if (!dynam) return M_PI; /* has no dynamics => static */
    
    const tdstDynamics* dynamics = pointer(dynam->dynamics);
    if (!dynamics) return M_PI; /* has no dynamics => static */
    
    /* Resolve the speed angle. The previous speed is same as the current speed. */
    const tdstVector3D speed = vector3_host_byteorder(dynamics->base.previousSpeed);
    const float sA = atan2(speed.y, speed.x);
    
    const tdstMatrix4D mA = fnActorGetMatrix(A);
    const tdstMatrix4D mB = fnActorGetMatrix(B);

    const tdstVector3D pA = game_matrix4_position(mA);
    const tdstVector3D pB = game_matrix4_position(mB);
    
    // Calculate the world coordinate space
    // angle to `B`from the perspective of `A`
    const float hR = atan2(pB.y - pA.y, pB.x - pA.x);
    // Then, calculate the relative angle of both
    float angle = hR - sA;
    
    if (angle <= -M_PI) angle =  (2.0f * M_PI + angle);
    if (angle >=  M_PI) angle = -(2.0f * M_PI - angle);
    return angle;
}

#pragma mark - StdGame

tdstStandardGameInfo *fnActorGetStdGame(const tdstEngineObject *object)
{
    return object ? pointer(object->stdGame) : NULL;
}

#pragma mark - Intelligence

tdstBrain* fnActorGetBrain(const tdstEngineObject *object)
{
    return object ? pointer(object->brain) : NULL;
}

tdstMind* fnActorGetMind(const tdstEngineObject *object)
{
    const tdstBrain *brain = fnActorGetBrain(object);
    return brain ? pointer(brain->mind) : NULL;
}

tdstIntelligence* fnActorGetIntelligence(const tdstEngineObject *object, bool reflex)
{
    const tdstMind *mind = fnActorGetMind(object);
    return mind ? pointer(reflex ? mind->reflex : mind->intelligence) : NULL;
}

tdstDsgMem* fnActorGetDsgMem(const tdstEngineObject *object)
{
    const tdstMind *mind = fnActorGetMind(object);
    return mind ? pointer(mind->dsgMem) : NULL;
}

void* fnActorGetDsgVar(const tdstEngineObject *object, uint8 var, uint8 *type)
{
    const tdstDsgMem *mem = fnActorGetDsgMem(object);
    return fnDsgMemGetDsgVar(mem, var, false, type);
}

/** fnActorGetName: return the family, model, or instance name of specified actor. Null on failure. */
const char* fnActorGetName(int name, const tdstEngineObject* actor, const tdstObjectType* objectType)
{
    if (!actor) return NULL;
    
    const tdstStandardGameInfo* stdgame = pointer(actor->stdGame);
    if (!stdgame) return NULL;
    
    switch (name)
    {
        case actor_family_name: return fnObjectTypeGetName(objectType, object_family_name, host_byteorder_32(stdgame->familyType));
        case actor_model_name: return fnObjectTypeGetName(objectType, object_model_name, host_byteorder_32(stdgame->modelType));
        case actor_instance_name: return fnObjectTypeGetName(objectType, object_instance_name, host_byteorder_32(stdgame->instanceType));
    }
    
    return NULL;
}

/** fnActorGetCurrentBehaviorName: return the name of the actor's current behavior */
const char* fnActorGetCurrentBehaviorName(const tdstEngineObject* actor)
{
    tdstIntelligence *intelligence = fnActorGetIntelligence(actor, false);
    const tdstBehavior* behavior = pointer(intelligence->currentBehavior);
    if (!behavior) return NULL;
    
    return behavior->name;
}

/** actor_in_behavior: return true if an actor is in the specified behavior */
bool fnActorIsInBehavior(const tdstEngineObject* actor, const char* behaviorName)
{
    const char* name = fnActorGetCurrentBehaviorName(actor);
    return name ? strcmp(name, behaviorName) == 0 : false;
}

#pragma mark - Find

tdstEngineObject *fnFindActor(int nameType, const char* name, tdstSuperObject* root)
{
    if (!root) return NULL;
    tdstEngineObject* actor = (tdstEngineObject*)superobject_data(root);
    if (superobject_type(root) == superobject_type_actor && actor)
    {
        const char* compare = fnActorGetName(nameType, actor, objectType);
        if (compare != NULL)
        {
            if (strcmp(name, compare) == 0) return actor;
        }
    }
    
    actor = NULL;
    tdstSuperObject* search = superobject_first_child(root);
    while (search && !actor)
    {
        actor = fnFindActor(nameType, name, search);
        search = superobject_next(search);
    }
    
    return actor;
}
