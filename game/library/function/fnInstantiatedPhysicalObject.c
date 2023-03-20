//
//  ipo.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include "stInstantiatedPhysicalObject.h"
#include "game.h"
#include "stCollideSet.h"
#include "stPhysicalObject.h"

/** ipo_name: get the name of an IPO */
const char* ipo_name(const tdstInstantiatedPhysicalObject* ipo)
{
    if (!ipo) return NULL;
    const char* name = memchr(ipo->name, ':', 0x32);
    return name ? name + 1 : NULL;
}

/** ipo_collide_object: get the collide object (ZDR) of an IPO */
const tdstCollideObject* ipo_collide_object(const tdstInstantiatedPhysicalObject* ipo)
{
    if (!ipo) return NULL;
    
    const tdstPhysicalObject* po = pointer(ipo->tdstPhysicalObject);
    if (!po) return NULL;
    
    const tdstPhysicalCollideSet* collset = pointer(po->physical_collideset);
    if (!collset) return NULL;
    
    /* Collision geometry is part of reaction zone. */
    const tdstCollideObject* zdr = pointer(collset->zdr);
    return zdr;
}
