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
const char* fnIPOGetName(const tdstInstantiatedPhysicalObject* ipo)
{
    if (!ipo) return NULL;
    const char* name = memchr(ipo->name, ':', 0x32);
    return name ? name + 1 : NULL;
}

/* ipo_world_matrix: compute the world transform of an IPO */
const tdstMatrix4D ipo_world_matrix(const tdstSuperObject* ipo)
{
    tdstMatrix4D result = matrix4_identity;
    tdstMatrix4D matrixStack[256];
    unsigned idx = 0;
    
    tdstSuperObject *parent = NULL;
    while ((parent = superobject_parent(ipo)))
        matrixStack[idx++] = fnSuperobjectGetGlobalMatrix(parent);
    
    while (--idx) result = matrix4_mul(matrixStack[idx], result);
    return matrix4_mul(fnSuperobjectGetGlobalMatrix(ipo), result);
}

/** fnIPOGetCollideObject: get the collide object (ZDR) of an IPO */
tdstCollideObject* fnIPOGetCollideObject(const tdstInstantiatedPhysicalObject* ipo)
{
    if (!ipo) return NULL;
    
    const tdstPhysicalObject* po = pointer(ipo->tdstPhysicalObject);
    if (!po) return NULL;
    
    const tdstPhysicalCollideSet* collset = pointer(po->physicalCollideset);
    if (!collset) return NULL;
    
    /* Collision geometry is part of reaction zone. */
    const tdstCollideObject* zdr = pointer(collset->zdr);
    return zdr;
}

#pragma mark - Find

tdstInstantiatedPhysicalObject* fnFindIPO(const char* name, tdstSuperObject *root, tdstSuperObject** so)
{
    if (!root) return NULL;
    tdstInstantiatedPhysicalObject* ipo = (tdstInstantiatedPhysicalObject*)superobject_data(root);
    if ((superobject_type(root) == superobject_type_ipo || superobject_type(root) == superobject_type_ipo_mirror) && ipo)
    {
        if (strcmp(name, ipo->name) == 0)
        {
            *so = root;
            return ipo;
        }
    }
    
    ipo = NULL;
    tdstSuperObject* search = superobject_first_child(root);
    while (search && !ipo)
    {
        ipo = fnFindIPO(name, search, so);
        search = superobject_next(search);
    }
    
    return ipo;
}
