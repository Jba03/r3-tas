#ifndef memory_h
#define memory_h

#include "configuration.h"
#include "endianness.h"

#include <stdint.h>

const extern uint8_t * memoryBase;

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

/** pointer: resolve memory pointer - expects game byteorder. */
#define pointer(addr) ((host_byteorder_32(addr) == 0x00) ? NULL : ((void*)(memoryBase + (host_byteorder_32(addr) & 0xFFFFFFF))))

/** doublepointer: resolve memory pointer, then resolve the resolved pointer. */
#define doublepointer(addr) pointer((((address*)pointer(addr) == NULL) ? 0x00 : *(address*)pointer(addr)))

/** offset: return the mRAM offset of specified structure */
#define offset(data) ((data == NULL) ? 0x0 : ((address)((uint8_t*)data - memoryBase)))

/** host_byteorder_f32: convert in-game float to platform byteorder */
static inline float host_byteorder_f32(uint32_t f)
{
    uint32_t v = host_byteorder_32(f);
    return (*(float*)(&v));
}

/** game_byteorder_f32: convert host-byteorder float to game byteorder */
static inline uint32_t game_byteorder_f32(float f)
{
    uint32_t v = *(uint32_t*)(&f);
    return game_byteorder_32(v);
}

/** convert an address to the game's addressing format */
#define game_address(addr) ((addr) | 0x80000000)

#endif /* memory_h */
