//
//  sector.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include "sector.h"
#include "game.h"

/** sector_name: get the abbreviated name of a sector */
const char* sector_name(const struct sector* sct)
{
    if (!sct) return NULL;
    const char* name = memchr(sct->name, ':', 0x104);
    return name ? name + 1 : NULL;
}
