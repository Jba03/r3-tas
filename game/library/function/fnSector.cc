//
//  sector.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-28.
//

#include "stSector.hh"
#include "game.hh"

/** fnSectorGetName: get the abbreviated name of a sector */
const char* fnSectorGetName(const tdstSector* sct)
{
    if (!sct) return NULL;
    const char* name = memchr(sct->name, ':', 0x104);
    return name ? name + 1 : NULL;
}
