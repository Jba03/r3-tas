//
//  transform.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "transform.h"
#include "stream.h"

TRANSFORM struct Transform transform_read(const address addr)
{
    struct Transform transform;
    transform.offset = addr;
    
    struct Stream *stream = stream_open(addr);
    transform.type = read32();
    advance(12); /* padding */
    transform.matrix = matrix4_read(stream->position);
    transform.scale = vector4_read(stream->position + 4 * 4 * 4);
    stream_close(stream);
    
    return transform;
}
