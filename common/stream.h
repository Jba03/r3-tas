//
//  stream.h
//  r3tools
//
//  Created by Jba03 on 2022-11-12.
//

#ifndef stream_h
#define stream_h

#include <string.h>
#include <stdlib.h>
#include "memory.h"

#define STREAM

struct Stream {
    
    uint8_t     (*read8)(struct Stream*);
    uint16_t    (*read16)(struct Stream*);
    uint32_t    (*read32)(struct Stream*);
    float       (*readfloat)(struct Stream*);
    const char* (*readstring)(struct Stream*, int length);
    const char* (*readstringN)(struct Stream*);
    
    void        (*write8)(struct Stream*, uint8_t value);
    void        (*write16)(struct Stream*, uint16_t value);
    void        (*write32)(struct Stream*, uint32_t value);
    void        (*writefloat)(struct Stream*, float value);
    const char* (*writestring)(struct Stream*, const char* string);
    
    void        (*seek)(struct Stream*, address offset);
    
    address position;
};

#define read8() stream->read8(stream)
#define read16() stream->read16(stream)
#define read32() stream->read32(stream)
#define read64() stream->read32(stream) // TODO: do.
#define read8S() (int8_t)read8()
#define read16S() (int16_t)read16()
#define read32S() (int32_t)read32()
#define readlong() read32() /* TODO: hmm */
#define readpointer() (read32() & 0xFFFFFFF)
#define readfloat() stream->readfloat(stream)
#define readstring(length) stream->readstring(stream, length)
#define readstringN() stream->readstringN(stream)

STREAM void stream_seek(struct Stream* stream, address offset);
STREAM struct Stream *stream_open(const address addr);
STREAM void stream_close(struct Stream *s);

#define advance(x) stream->position += (x)

#endif /* stream_h */
