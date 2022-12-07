//
//  graphnode.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#ifndef graphnode_h
#define graphnode_h

#include "global.h"
#include "waypoint.h"

#define GRAPH_NODE

struct GraphNode {
    pointer ptr_next;
    pointer ptr_prev;
    pointer ptr_graph;
    pointer ptr_waypoint;
    pointer ptr_arclist;
    
    struct GraphNode* next;
    struct GraphNode* prev;
    struct Graph* graph;
    struct WayPoint* waypoint;
    
    int waypoint_type;
    int waypoint_type_initial;
    
    address offset;
};

GRAPH_NODE struct GraphNode* graphnode_read(const address addr);

#endif /* graphnode_h */
