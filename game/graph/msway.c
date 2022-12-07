//
//  msway.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-07.
//

#include "msway.h"

MS_WAY struct MSWay* msway_read(const address addr)
{
    struct MSWay* way = malloc(sizeof *way);
    way->ptr_graph = memory.read_32(addr + 0 * 4) & 0xFFFFFFF;
    way->index = memory.read_32(addr + 1 * 4);
    way->c = memory.read_32(addr + 2 * 4);
    
    if (way->ptr_graph != 0x00) way->graph = graph_read(way->ptr_graph);
    
    return way;
}
