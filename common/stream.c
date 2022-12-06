//
//  stream.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "stream.h"
#include "logging.h"
#include "memory.h"

static uint8_t stream_read8(struct Stream *s)   { uint8_t  v = memory.read_8(s->position);      s->position += 1; return v; }
static uint16_t stream_read16(struct Stream *s) { uint16_t v = memory.read_16(s->position);     s->position += 2; return v; }
static uint32_t stream_read32(struct Stream *s) { uint32_t v = memory.read_32(s->position);     s->position += 4; return v; }
static float stream_readfloat(struct Stream *s) { float    v = memory.read_float(s->position);  s->position += 4; return v; }

static void stream_write8(struct Stream *s, uint8_t v)   { memory.write_8(v, s->position);      s->position += 1; }
static void stream_write16(struct Stream *s, uint16_t v) { memory.write_16(v, s->position);     s->position += 2; }
static void stream_write32(struct Stream *s, uint32_t v) { memory.write_32(v, s->position);     s->position += 4; }
static void stream_writefloat(struct Stream *s, float v) { memory.write_float(v, s->position);  s->position += 4; }

static const char* stream_readstringN(struct Stream *stream)
{
    int l = 0;
    char str[1024]; memset(str, 0, 1024);
    while ((str[l++] = read8()) != '\0');
    return strdup(str);
}

static const char* stream_readstring(struct Stream *stream, int l)
{
    if (l <= 0) return NULL;
    char str[l];
    int c = l;
    while (c--) str[l-c-1] = read8();
    return strdup(str);
}

static void stream_register_rw_functions(struct Stream *stream)
{
    stream->read8 = stream_read8;
    stream->read16 = stream_read16;
    stream->read32 = stream_read32;
    stream->readfloat = stream_readfloat;
    stream->readstring = stream_readstring;
    stream->readstringN = stream_readstringN;
    
    stream->write8 = stream_write8;
    stream->write16 = stream_write16;
    stream->write32 = stream_write32;
    stream->writefloat = stream_writefloat;
}

STREAM void stream_seek(struct Stream* stream, address offset)
{
    stream->position = offset;
}

STREAM struct Stream *stream_open(const uint32_t address)
{
    struct Stream *stream = (struct Stream*)malloc(sizeof *stream);
    stream->position = address;
    stream->seek = stream_seek;
    
    stream_register_rw_functions(stream);
    
    return stream;
}

STREAM void stream_close(struct Stream *s)
{
    free(s);
}
