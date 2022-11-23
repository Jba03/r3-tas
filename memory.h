//
//  memory.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef memory_h
#define memory_h

#include <stdint.h>

typedef uint32_t pointer;
typedef uint32_t address;

#define GCN_POINTER_FIX         0x004334cc /* GCN location of FIX */
#define GCN_POINTER_LVL         0x004334d0 /* GCN location of LVL */
#define GCN_POINTER_ENGINE      0x003E7C0C /* GCN location of engine structure */
#define GCN_POINTER_HIERARCHY1  0x004334d4 /* GCN Location of superobject hierarchy pointer 1 */
#define GCN_POINTER_HIERARCHY2  0x004334d8 /* GCN location of superobject hierarchy pointer 2 */

struct Memory {
    pointer pointer_fix;
    pointer pointer_lvl;
    pointer pointer_engine;
    pointer pointer_hierarchy1;
    pointer pointer_hierarchy2;
    
    uint8_t (*read_8)(address address);
    uint16_t (*read_16)(address address);
    uint32_t (*read_32)(address address);
    float (*read_float)(address address);
    void (*write_8)(uint8_t value, address address);
    void (*write_16)(uint16_t value, address address);
    void (*write_32)(uint32_t value, address address);
    void (*write_float)(float value, address address);
};

extern struct Memory memory;

#endif /* memory_h */
