//
//  vertex.h
//  r3tools
//
//  Created by Jba03 on 2022-11-23.
//

#ifndef vertex_h
#define vertex_h

#include "vector3.h"

struct GLVertex {
    struct { float x, y, z; } position;
    struct { float x, y, z; } normal;
};

#endif /* vertex_h */
