#ifndef structure_h
#define structure_h

#include <stdint.h>
#include <stdbool.h>

#include "endianness.h"

#define GCN 0
#define PS2 1
#define PC  2
#define platform GCN

#define USE_FUNCTIONS   1

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

#define readonly // const

typedef char        char8;
typedef uint8_t     uchar8;
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

/* macros to synthesize unique struct padding field names */
#define concat(a, b) concat_inner(a, b)
#define concat_inner(a, b) a ## b
#define unique_name(base) concat(base, __LINE__)

/** padding: insert padding bytes. */
#define padding(S) readonly uint8_t unique_name(padding) [S];
/** pointer: resolve memory pointer - expects game byteorder. */
#define pointer(addr) ((host_byteorder_32(addr) == 0x00) ? NULL : ((void*)(memory.base + (host_byteorder_32(addr) & 0xFFFFFFF))))
/** doublepointer: resolve memory pointer, then resolve the resolved pointer. */
#define doublepointer(addr) pointer((((address*)pointer(addr) == NULL) ? 0x00 : *(address*)pointer(addr)))
/** offset: return the mRAM offset of specified structure */
#define offset(data) ((data == NULL) ? 0x0 : ((address)((uint8_t*)data - memory.base)))
/** host_byteorder_f32: convert in-game float to platform byteorder */
static inline float host_byteorder_f32(float32 f)
{
    uint32_t v = host_byteorder_32(f);
    return (*(float*)(&v));
}
/** game_byteorder_f32: convert host-byteorder float to game byteorder */
static inline float32 game_byteorder_f32(float f)
{
    uint32_t v = *(uint32_t*)(&f);
    return game_byteorder_32(v);
}

/** convert an address to the game's addressing format */
#define game_address(addr) ((addr) | 0x80000000)

#endif /* structure_h */
