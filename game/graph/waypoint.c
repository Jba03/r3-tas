//
//  waypoint.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#include "waypoint.h"

WAYPOINT struct WayPoint* waypoint_read(const address addr)
{
    struct WayPoint* wp = malloc(sizeof *wp);
    wp->offset = addr;
    
    wp->position = vector3_read(addr + 0);
    wp->radius = memory.read_float(addr + 3 * 4);
    
    return wp;
}
