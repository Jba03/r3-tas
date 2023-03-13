//
//  superobject.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-18.
//

#include "game.h"
#include "superobject.h"
#include "transform.h"
#include "sector.h"
#include "intersect.h"

/** superobject_typename: get the typename of a superobject */
const char* superobject_typename(const struct superobject* so)
{
    if (!so) return "None";
    const int type = superobject_type(so);
    const int n_typenames = sizeof superobject_typenames / sizeof(const char*);
    if (type > 0 && type < n_typenames) return superobject_typenames[((int)log2(type))];
    return "None";
}

/** superobject_name: get the (instance) name of a superobject*/
const char* superobject_name(const struct superobject* so)
{
    if (!so) return NULL;
    const int type = superobject_type(so);
    const void* data = superobject_data(so);
    switch (type)
    {
        case superobject_type_actor:
        {
            const char* name = actor_name(actor_instance_name, data);
            if (!name) name = actor_name(actor_model_name, data);
            if (!name) name = actor_name(actor_family_name, data);
            return name;
        }
            
        case superobject_type_ipo: return ipo_name(data);
        case superobject_type_sector: return sector_name(data);
    }
    
    return NULL;
}

/** superobject_matrix_global: get the global world transform matrix of a superobject */
const struct matrix4 superobject_matrix_global(const struct superobject* so)
{
    if (!so) return matrix4_identity;
    const struct transform* transform = pointer(so->transform_global);
    if (!transform) return matrix4_identity;
    return matrix4_host_byteorder(transform->matrix);
}

/** superobject_matrix_local: get the local world transform matrix of a superobject */
const struct matrix4 superobject_matrix_local(const struct superobject* so)
{
    if (!so) return matrix4_identity;
    const struct transform* transform = pointer(so->transform_local);
    if (!transform) return matrix4_identity;
    return matrix4_host_byteorder(transform->matrix);
}

/** sector_by_location: get the sector in which the specified point is located */
const struct superobject* sector_by_location(const struct superobject* father_sector, const struct vector3 point)
{
    if (!father_sector) return NULL;
    
    float d_near = INFINITY;
    float d_current = INFINITY;
    float d_virtual = INFINITY;
    int8 p = sector_priority_min;
    int8 v = sector_priority_min;
 
    struct superobject* target_sector = NULL;
    struct superobject* target_sector_virtual = NULL;
    
    superobject_for_each(father_sector, so)
    {
        const struct sector* subsector = superobject_data(so);
        if (!subsector) continue;
        
        const struct vector3 min = vector3_host_byteorder(subsector->min);
        const struct vector3 max = vector3_host_byteorder(subsector->max);
        
        if (intersect_box_point(min, max, point))
        {
            struct vector3 distance;
            distance = vector3_add(min, max);
            distance = vector3_mulf(distance, 1.0f / 2.0f);
            distance = vector3_sub(distance, point);
            d_near = vector3_length(distance);
            
            if (!subsector->is_virtual)
            {
                if (subsector->priority > p)
                {
                    target_sector = so;
                    d_current = d_near;
                    p = subsector->priority;
                }
                else if (subsector->priority == p && d_near < d_current)
                {
                    target_sector = so;
                    d_current = d_near;
                }
            }
            else
            {
                if (subsector->priority > v)
                {
                    target_sector_virtual = so;
                    d_virtual = d_near;
                    v = subsector->priority;
                }
                else if (subsector->priority == v && d_near < d_virtual)
                {
                    target_sector_virtual = so;
                    d_virtual = d_near;
                }
            }
        }
    };
    
    if (!target_sector) target_sector = target_sector_virtual;
    if (!target_sector) target_sector = superobject_last_child(father_sector); /* UNIVERS */
    return target_sector;
}
