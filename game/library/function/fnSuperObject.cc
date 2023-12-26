//
//  superobject.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-18.
//

#include "game.hh"
#include "memory.hh"
#include "stSuperObject.hh"
#include "stInstantiatedPhysicalObject.hh"
#include "stTransform.hh"
#include "stSector.hh"
#include "fnPrimIntersections.c"

#include "fnTransform.c"

const char* fnSuperobjectGetTypename(const stSuperObject* so)
{
    if (!so) return "None";
    const int type = superobject_type(so);
    const int n_typenames = sizeof superObjectTypeNames / sizeof(const char*);
    if (type > 0 && type < n_typenames) return superObjectTypeNames[((int)log2(type))];
    return "None";
}

const char* fnSuperobjectGetName(const stSuperObject* so)
{
    if (!so) return NULL;
    const int type = superobject_type(so);
    const void* data = superobject_data(so);
    switch (type)
    {
        case superobject_type_actor:
        {
            const char* name = fnActorGetName(actor_instance_name, data, objectType);
            if (!name) name = fnActorGetName(actor_model_name, data, objectType);
            if (!name) name = fnActorGetName(actor_family_name, data, objectType);
            return name;
        }
            
        case superobject_type_ipo: return fnIPOGetName(data);
        case superobject_type_sector: return fnSectorGetName(data);
    }
    
    return NULL;
}

void *fnSuperobjectGetData(const stSuperObject *so)
{
    return so ? pointer(so->data) : NULL;
}

stTransform* fnSuperobjectGetTransform(const stSuperObject *obj, bool local)
{
    return obj ? pointer(local ? obj->localTransform : obj->globalTransform) : NULL;
}

stMatrix4D fnSuperobjectGetGlobalMatrix(const stSuperObject *obj)
{
    return fnTransformGetMatrix(fnSuperobjectGetTransform(obj, false));
}

stMatrix4D fnSuperobjectGetLocalMatrix(const stSuperObject* obj)
{
    return fnTransformGetMatrix(fnSuperobjectGetTransform(obj, true));
}

/** sector_by_location: get the sector in which the specified point is located */
const stSuperObject* sector_by_location(const stSuperObject* father_sector, const stVector3D point)
{
    if (!father_sector) return NULL;
    
    float d_near = INFINITY;
    float d_current = INFINITY;
    float d_virtual = INFINITY;
    int8 p = sector_priority_min;
    int8 v = sector_priority_min;
 
    stSuperObject* target_sector = NULL;
    stSuperObject* target_sector_virtual = NULL;
    
    superobject_for_each(father_sector, so)
    {
        const stSector* subsector = superobject_data(so);
        if (!subsector) continue;
        
        const stVector3D min = vector3_host_byteorder(subsector->min);
        const stVector3D max = vector3_host_byteorder(subsector->max);
        
        if (fnBoxPointIntersection(min, max, point))
        {
            stVector3D distance;
            distance = vector3_add(min, max);
            distance = vector3_mulf(distance, 1.0f / 2.0f);
            distance = vector3_sub(distance, point);
            d_near = vector3_length(distance);
            
            if (!subsector->isVirtual)
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
