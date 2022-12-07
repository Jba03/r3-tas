//
//  msway.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#ifndef msway_h
#define msway_h

#include "graph.h"

#define MS_WAY

struct MSWay {
    pointer ptr_graph;
    struct Graph* graph;
    int index;
    bool c;
};

MS_WAY struct MSWay* msway_read(const address addr);

#endif /* msway_h */
