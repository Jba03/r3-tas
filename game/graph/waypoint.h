//
//  waypoint.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#ifndef waypoint_h
#define waypoint_h

#include "global.h"

#define WAYPOINT

struct WayPoint {
    pointer ptr_superobject;
    
    struct Vector3 position;
    float radius;
    struct SuperObject* superobject;
    
    address offset;
};

WAYPOINT struct WayPoint* waypoint_read(const address addr);

#endif /* waypoint_h */
