//
//  route.h
//  r3-tas
//
//  Created by Jba03 on 2023-03-20.
//

#ifndef route_h
#define route_h

#include "stVector3D.h"

struct route {
    tdstVector3D* route;
    unsigned int length, sz;
    unsigned int complete_time, dt;
    tdstVector3D color;
};

void initializeRoute(struct route *route);

void recordRoute(struct route *route, tdstVector3D position);

void finishRoute(struct route *route);

#endif /* route_h */
