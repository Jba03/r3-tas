//
//  route.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-20.
//

#include "route.h"
#include <stdlib.h>

#define ROUTE_INITIAL_LENGTH (1 << 10)

void initializeRoute(struct route *route)
{
    route->route = malloc(sizeof(tdstVector3D) * ROUTE_INITIAL_LENGTH);
    route->length = 0;
    route->complete_time = 0;
    route->dt = 2;
    route->sz = 1;
}

void recordRoute(struct route *route, tdstVector3D position)
{
    ++route->length;
    if (route->length > route->sz * ROUTE_INITIAL_LENGTH)
    {
        route->sz *= 2;
        route->route = realloc(route->route, route->sz * sizeof(tdstVector3D) * ROUTE_INITIAL_LENGTH);
    }
    
    route->route[route->length - 1] = position;
    route->complete_time = route->dt * route->length;
}

void finishRoute(struct route *route)
{
    free(route->route);
}
