//
//  transform.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "transform.h"
#include "stream.h"

TRANSFORM struct Transform* transform_read(const address addr)
{
    struct Transform* transform = malloc(sizeof *transform);
    transform->offset = addr;
    
    struct Stream *stream = stream_open(addr);
    transform->type = read32();
//    transform->matrix = matrix4_read(stream);
//    transform->scale = vector4_read(stream);
    stream_close(stream);
    
    return transform;
}
