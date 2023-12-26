//
//  ipo.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include <string.h>

#include "stInstantiatedPhysicalObject.hh"
#include "memory.hh"
#include "stCollideSet.hh"
#include "stPhysicalObject.hh"

/** fnIPOGetName: get the name of an IPO */
const char* fnIPOGetName(const stInstantiatedPhysicalObject* ipo)
{
    if (!ipo) return NULL;
    const char* name = memchr(ipo->name, ':', 0x32);
    return name ? name + 1 : NULL;
}

/* ipo_world_matrix: compute the world transform of an IPO */
const stMatrix4D ipo_world_matrix(const stSuperObject* ipo)
{
    stMatrix4D result = matrix4_identity;
    stMatrix4D matrixStack[256];
    unsigned idx = 0;
    
    stSuperObject *parent = NULL;
    while ((parent = superobject_parent(ipo)))
        matrixStack[idx++] = fnSuperobjectGetGlobalMatrix(parent);
    
    while (--idx) result = matrix4_mul(matrixStack[idx], result);
    return matrix4_mul(fnSuperobjectGetGlobalMatrix(ipo), result);
}

/** fnIPOGetCollideObject: get the collide object (ZDR) of an IPO */
stCollideObject* fnIPOGetCollideObject(const stInstantiatedPhysicalObject* ipo)
{
    if (!ipo) return NULL;
    
    const stPhysicalObject* po = pointer(ipo->stPhysicalObject);
    if (!po) return NULL;
    
    const stPhysicalCollideSet* collset = pointer(po->physicalCollideset);
    if (!collset) return NULL;
    
    /* Collision geometry is part of reaction zone. */
    const stCollideObject* zdr = pointer(collset->zdr);
    return zdr;
}

#pragma mark - Find

stInstantiatedPhysicalObject* fnFindIPO(const char* name, stSuperObject *root, stSuperObject** so)
{
    if (!root) return NULL;
    stInstantiatedPhysicalObject* ipo = (stInstantiatedPhysicalObject*)superobject_data(root);
    if ((superobject_type(root) == superobject_type_ipo || superobject_type(root) == superobject_type_ipo_mirror) && ipo)
    {
        if (strcmp(name, ipo->name) == 0)
        {
            *so = root;
            return ipo;
        }
    }
    
    ipo = NULL;
    stSuperObject* search = superobject_first_child(root);
    while (search && !ipo)
    {
        ipo = fnFindIPO(name, search, so);
        search = superobject_next(search);
    }
    
    return ipo;
}
