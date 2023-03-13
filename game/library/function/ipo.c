//
//  ipo.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include "ipo.h"
#include "game.h"
#include "collideset.h"
#include "physical_object.h"

/** ipo_name: get the name of an IPO */
const char* ipo_name(const struct ipo* ipo)
{
    if (!ipo) return NULL;
    const char* name = memchr(ipo->name, ':', 0x32);
    return name ? name + 1 : NULL;
}

/** ipo_collide_object: get the collide object (ZDR) of an IPO */
const struct collide_object* ipo_collide_object(const struct ipo* ipo)
{
    if (!ipo) return NULL;
    
    const struct physical_object* po = pointer(ipo->physical_object);
    if (!po) return NULL;
    
    const struct physical_collideset* collset = pointer(po->physical_collideset);
    if (!collset) return NULL;
    
    /* Collision geometry is part of reaction zone. */
    const struct collide_object* zdr = pointer(collset->zdr);
    return zdr;
}
