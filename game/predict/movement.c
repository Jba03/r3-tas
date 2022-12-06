//
//  movement.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#include "movement.h"

static bool point_in_sphere(struct Vector3 point, struct Vector3 sphere_center, const float sphere_radius)
{
    return false;
}

static struct Vector3 find_closest_ground_point(struct prediction_param* param)
{
    return vector3_new(0, 0, 0);
}

static void predict_air_movement(struct prediction_param* param, struct route* result)
{
    struct Actor* origin = param->i_source;
}

static void predict_ground_movement(struct prediction_param* param, struct route* result)
{
    
}

struct route* predict_movement_optimal_route(struct prediction_param* param)
{
    if (!param->i_engine) return NULL;
    if (!param->i_source) return NULL;
    
    /* Re-read dynamics */
    uint32_t addr = param->i_source->dynamics->offset;
    free(param->i_source->dynamics);
    param->i_source->dynamics = dynamics_read(addr);
    
    /* Assume the source object is dynamic, and therefore has a dynamics structure */
    param->source_position = param->i_source->dynamics->base.anim_speed;
    /* For now, set the direction of the source object as its speed normalized. */
    param->source_direction = vector3_normalize(param->i_source->dynamics->base.speed_impose);
    
    struct route* result = malloc(sizeof *result);
    
    switch (param->i_mode)
    {
        case MODE_IN_AIR:
            predict_air_movement(param, result);
            break;
            
        case MODE_ON_GROUND:
            predict_ground_movement(param, result);
            break;
            
        default:
            error(BOLD "[predict] mode not set.");
            return NULL;
            break;
    }
    
    return result;
}
