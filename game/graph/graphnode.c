//
//  graphnode.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#include "graphnode.h"

GRAPH_NODE struct GraphNode* graphnode_read(const address addr)
{
    struct GraphNode* node = malloc(sizeof *node);
    node->offset = addr;
    node->next = NULL;
    node->prev = NULL;
    node->graph = NULL;
    node->waypoint = NULL;
    
    struct Stream* stream = stream_open(addr);
    node->ptr_next = readpointer();
    node->ptr_prev = readpointer();
    node->ptr_graph = readpointer();
    node->ptr_waypoint = readpointer();
    node->waypoint_type = read32();
    node->waypoint_type_initial = read32();
    stream_close(stream);
    
    if (node->ptr_waypoint != 0x00) node->waypoint = waypoint_read(node->ptr_waypoint);
    
    return node;
}
