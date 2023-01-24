//
//  vertex.h
//  r3tools
//
//  Created by Jba03 on 2022-11-23.
//

#ifndef vertex_h
#define vertex_h

//#include "vector3.h"

struct vertex
{
    struct { float x, y, z, w; } position;
    struct { float x, y, z, w; } normal;
    struct { float x, y, z, w; } texcoord;
};

#endif /* vertex_h */
