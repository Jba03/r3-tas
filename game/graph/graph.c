//
//  graph.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#include "graph.h"

GRAPH struct Graph* graph_read(const address addr)
{
    struct Graph* graph = malloc(sizeof *graph);
    graph->offset = addr;
    graph->node_first = NULL;
    graph->node_last = NULL;
    graph->node = NULL;
    
    struct Stream* stream = stream_open(addr);
    graph->ptr_node_first = readpointer();
    graph->ptr_node_last = readpointer();
    graph->n_nodes = read32();
    
    if (graph->n_nodes > 0 && graph->ptr_node_first != 0x00)
    {
        graph->node = malloc(sizeof(struct GraphNode*) * graph->n_nodes);
        
        pointer ptr_next = graph->ptr_node_first;
        for (unsigned n = 0; n < graph->n_nodes; n++)
        {
            graph->node[n] = graphnode_read(ptr_next);
            ptr_next = graph->node[n]->ptr_next;

            if (ptr_next == 0x00)
                break;
        }

        graph->node_first = graph->node[0];
        graph->node_last = graph->node[graph->n_nodes - 1];
    }
    
    stream_close(stream);
    
    /* Add to global graph list */
    array_push(graph_list, graph);
    
    return graph;
}
