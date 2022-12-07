//
//  graph.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#ifndef graph_h
#define graph_h

#include "global.h"
#include "graphnode.h"

#define GRAPH

struct Graph {
    pointer ptr_node_first;
    pointer ptr_node_last;
    
    struct GraphNode* node_first;
    struct GraphNode* node_last;
    struct GraphNode** node;
    int n_nodes;
    
    address offset;
};

GRAPH struct Graph* graph_read(const address addr);

#endif /* graph_h */
