#ifndef structure_h
#define structure_h

#include <stdint.h>
#include <stdbool.h>

#include "conf.h"

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

#define readonly // const

/* macros to synthesize unique struct padding field names */
#define concat(a, b) concat_inner(a, b)
#define concat_inner(a, b) a ## b
#define unique_name(base) concat(base, __LINE__)

/** padding: insert padding bytes. */
#define padding(S) readonly uint8_t unique_name(padding) [S];

#define USE_FUNCTIONS

#endif /* structure_h */
