//
//  actor.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-19.
//

#include "stEngineObject.h"
#include "stBrain.h"
#include "stMind.h"
#include "stIntelligence.h"
#include "stBehavior.h"
#include "stStandardGameInfo.h"
#include "stDynamics.h"
#include "stDsg.h"
#include "stObjectType.h"
#include "game.h"

#include <stdio.h>

/** actor_matrix: get the transformation matrix of an actor */
const tdstMatrix4D actor_matrix(const tdstEngineObject* actor)
{
    if (!actor) return matrix4_identity;
    const tdstSuperObject* so = actor_superobject(actor);
    return superobject_matrix_global(so);
}

/** actor_position: get global position of an actor */
const tdstVector3D actor_position(const tdstEngineObject* actor)
{
    const tdstMatrix4D T = actor_matrix(actor);
    return game_matrix4_position(T);
}

/** actor_speed: get current speed of an actor */
const tdstVector3D actor_speed(const tdstEngineObject* actor)
{
    const tdstDynamics* dynamics = actor_dynamics(actor);
    return dynamics_get_speed(dynamics);
}

/**
 * actor_horizontal_sighting_relative:
 *  compute the horizontal angle between actor A's line of sight and the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float actor_horizontal_sighting_relative(const tdstEngineObject* A, const tdstEngineObject* B)
{
    const tdstMatrix4D mA = actor_matrix(A);
    const tdstMatrix4D mB = actor_matrix(B);

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
 * actor_vertical_sighting_relative:
 *  compute the vertical angle between actor A's line of sight and the position of actor B.
 *  the angle returned is in radians, range -π to π.
 */
const float actor_vertical_sighting_relative(const tdstEngineObject* A, const tdstEngineObject* B)
{
    const tdstMatrix4D mA = actor_matrix(A);
    const tdstMatrix4D mB = actor_matrix(B);

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
const float actor_trajectory_angle_relative(const tdstEngineObject* A, const tdstEngineObject* B)
{
    if (!A || !B) return M_PI;
    
    const tdstDynam* dynam = actor_dynam(A);
    if (!dynam) return M_PI; /* has no dynamics => static */
    
    const tdstDynamics* dynamics = pointer(dynam->dynamics);
    if (!dynamics) return M_PI; /* has no dynamics => static */
    
    /* Resolve the speed angle. The previous speed is same as the current speed. */
    const tdstVector3D speed = vector3_host_byteorder(dynamics->base.speed_previous);
    const float sA = atan2(speed.y, speed.x);
    
    const tdstMatrix4D mA = actor_matrix(A);
    const tdstMatrix4D mB = actor_matrix(B);

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

/** actor_dsgvar: get the offset of an actor's dsgvar. Negative on failure. */
const int actor_dsgvar(const tdstEngineObject* actor, unsigned var, int* type, void** data)
{
    if (!actor) return -1;
    if (!data) return -1;
    
    const tdstBrain* brain = actor_brain(actor);
    if (!brain) return -1;
    
    const tdstMind* mind = pointer(brain->mind);
    if (!mind) return -1;
    
    const tdstDsgMem* dsgmem = pointer(mind->dsgmemory);
    if (!dsgmem) return -1;
    
    const tdstDsgVar* dsgvars = doublepointer(dsgmem->dsgvars);
    if (!dsgvars) return -1;
    
    if (var >= dsgvars->info_length) return -1;
    
    const tdstDsgVarInfo* variable = (tdstDsgVarInfo*)pointer(dsgvars->info) + var;
    if (type) *type = host_byteorder_32(variable->type);
    
    const uint8_t* dataptr = (uint8_t*)pointer(dsgmem->buffer_current);
    if (!dataptr) return -1;
    
    *data = (void*)(dataptr + host_byteorder_32(variable->mem_offset));
    return 0;
}

/** actor_name: return the family, model, or instance name of specified actor. Null on failure. */
const char* actor_name(int name, const tdstEngineObject* actor, const tdstObjectType* objectType)
{
    if (!actor) return NULL;
    
    const tdstStandardGameInfo* stdgame = pointer(actor->stdgame);
    if (!stdgame) return NULL;
    
    switch (name)
    {
        case actor_family_name: return object_type_name(objectType, object_family_name, host_byteorder_32(stdgame->family_type));
        case actor_model_name: return object_type_name(objectType, object_model_name, host_byteorder_32(stdgame->model_type));
        case actor_instance_name: return object_type_name(objectType, object_instance_name, host_byteorder_32(stdgame->instance_type));
    }
    
    return NULL;
}

/** actor_current_behavior_name: return the name of the actor's current behavior */
const char* actor_current_behavior_name(const tdstEngineObject* actor)
{
    if (!actor) return NULL;
    
    const tdstBrain* brain = actor_brain(actor);
    if (!brain) return NULL;
    
    const tdstMind* mind = pointer(brain->mind);
    if (!mind) return NULL;
    
    const tdstIntelligence* intelligence = pointer(mind->intelligence);
    if (!intelligence) return NULL;
    
    const tdstBehavior* behavior = pointer(intelligence->current_behavior);
    if (!behavior) return NULL;
    
    return behavior->name;
}

/** actor_in_behavior: return true if an actor is in the specified behavior */
bool actor_in_behavior(const tdstEngineObject* actor, const char* behavior_name)
{
    if (!actor) return NULL;
    
    const tdstBrain* brain = actor_brain(actor);
    if (!brain) return NULL;
    
    const tdstMind* mind = pointer(brain->mind);
    if (!mind) return NULL;
    
    const tdstIntelligence* intelligence = pointer(mind->intelligence);
    if (!intelligence) return NULL;

    const tdstBehavior* behavior = pointer(intelligence->current_behavior);
    if (!behavior) return NULL;
    
    return strcmp(behavior->name, behavior_name) == 0;
}

/** actor_dynamics: get dynamics structure of an actor */
const tdstDynamics* actor_dynamics(const tdstEngineObject* actor)
{
    const tdstDynam* dynam = pointer(actor->dynam);
    return dynam ? pointer(dynam->dynamics) : NULL;
}

/** actor_dynamics_report: get dynamics report of an actor */
const tdstDynamicsReport* actor_dynamics_report(const tdstEngineObject* actor)
{
    const tdstDynamics* dynamics = actor_dynamics(actor);
    return dynamics ? pointer(dynamics->base.report) : NULL;
}
