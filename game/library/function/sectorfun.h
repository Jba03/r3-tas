//
//  sectorfun.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-23.
//

#ifndef sectorfun_h
#define sectorfun_h

#include "superobject.h"

static inline struct superobject* sector_get_active(const struct superobject* fathersector, const vector3 position)
{
    const struct sector* sector_last;
    
    superobject_for_each(fathersector, sect)
    {
        const struct sector* sector = superobject_sector(sect);
        if (!sector) continue;
        
        last = sector;
        
        
    }
}

#endif /* sectorfun_h */
