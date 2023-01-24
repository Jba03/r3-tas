#ifndef structure_h
#define structure_h

#include <stdint.h>
#include <stdbool.h>

//#include "vector3.h"
#include "endianness.h"

#define GCN 0
#define PC  1
#define platform GCN

#define swap16(data) \
    ((((data) >> 8) & 0x00FF) | (((data) << 8) & 0xFF00))

#define swap32(data) \
    ((((data) >> 24) & 0x000000FF) | (((data) >>  8) & 0x0000FF00) | \
    ( ((data) <<  8) & 0x00FF0000) | (((data) << 24) & 0xFF000000) )

#ifdef MSB_FIRST
#
#   define host_byteorder_16(data) (data)
#   define host_byteorder_32(data) (data)
#   define game_byteorder_16(data) (data)
#   define game_byteorder_32(data) (data)
#
#else /* LSB_FIRST */
#
#   define host_byteorder_16(data) swap16(data)
#   define host_byteorder_32(data) swap32(data)
#   define game_byteorder_16(data) swap16(data)
#   define game_byteorder_32(data) swap32(data)
#
#endif

/* generate structure operators? */
#define structure_operators

/* allow structure modifications? */
#define readonly

/* TODO: Add platform-specific typedefs once char type name is figured out */


typedef int8_t      int8;
typedef uint8_t     uint8;
typedef int16_t     int16;
typedef uint16_t    uint16;
typedef int32_t     int32;
typedef uint32_t    uint32;
typedef uint32_t    float32;
typedef uint32_t    address;
typedef uint32_t    pointer;
typedef uint32_t    doublepointer;

// typedef float struct vector3[3];
// typedef float vector4[4];

#define align(S) __attribute__ ((aligned(S), packed))

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

/** padding: insert padding bytes. */
#define padding(S) readonly uint8_t UNIQUE_NAME(padding) [S];
/** pointer: resolve memory pointer. Expects game byteorder. */
#define pointer(addr) ((host_byteorder_32(addr) == 0x00) ? NULL : ((void*)(memory.base + (host_byteorder_32(addr) & 0xFFFFFFF))))
/** doublepointer: resolve memory pointer, then resolve that pointer. */
#define doublepointer(addr) pointer(*(pointer*)pointer(addr))
/** offset: return the mRAM offset of specified structure */
#define offset(data) ((data == NULL) ? 0x0 : ((address)((uint8_t*)data - memory.base)))

static inline float host_byteorder_f32(float32 f)
{
    uint32_t v = host_byteorder_32(f);
    return (*(float*)(&v));
}

#include "engine.h"
#include "engine_timer.h"
#include "superobject.h"
#include "actor.h"

#endif /* structure_h */
