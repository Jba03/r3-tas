//
//  predict.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-24.
//

#include "predict.h"
#include "common.h"
#include "brain.h"
#include "dsg.h"

#define RAYMAN_IN_AIR_DSGVAR    95

const struct Vector3 closest_point_on_segment(struct Vector3 point, struct predicted_vector s)
{
    const struct Vector3 L = vector3_sub(s.end, s.start);
    const struct Vector3 direction = vector3_normalize(L);
    const struct Vector3 v = vector3_sub(point, s.start);
    
    float d = vector3_dot(v, direction);
    d = min(max(0.0f, d), vector3_length(L));
    
    return vector3_add(s.start, vector3_mulf(direction, d));
}

const float distance_point_to_segment(struct Vector3 point, struct predicted_vector S)
{
    struct Vector3 p = closest_point_on_segment(point, S);
    struct Vector3 d = vector3_sub(point, p);
    return vector3_length(d);
}

static void mesh_list_append(struct SuperObject* ipo, void* array)
{
    array_push(array, ipo);
}

struct prediction_param predict_determine_default_param(struct Engine* engine)
{
    struct prediction_param param;
    param.i_engine = engine;
    param.i_mode = MODE_NONE;
    
    /* Find rayman and set him as origin. */
    if (!(param.i_source = actor_find(ACTOR_FIND_FAMILY, "rayman", engine->root)))
    {
        warning("[predict] could not construct default param: rayman not found\n");
        return param;
    }
        
    /* Find the level end trigger and set it as final target */
    if (!(param.i_target_final = actor_find(ACTOR_FIND_MODEL, "NIN_m_ChangeMap", engine->root)))
    {
        warning("[predict] could not construct default param: level end trigger not found\n");
        return param;
    }
        
    struct DSGMemory* dsg = param.i_source->brain->mind->dsg;
    struct DSGVariableInfo info = dsg->current[RAYMAN_IN_AIR_DSGVAR];
    bool airborn = memory.read_8(info.data_offset) != 0;
    
    if (airborn)
        param.i_mode = MODE_IN_AIR;
    else
        param.i_mode = MODE_ON_GROUND;
    
    param.mesh_list = array_create();
    
    //superobject_for_each(SUPEROBJECT_TYPE_IPO, engine->root, &mesh_list_append, param.mesh_list);
    
    
    return param;
}
