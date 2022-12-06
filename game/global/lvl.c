//
//  lvl.c
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#include "lvl.h"
#include "fix.h"
#include "global.h"

#include <stdio.h>
#include <stdlib.h>

LVL_STRUCTURE struct LVL* lvl_read(const address addr)
{
    struct LVL* lvl = malloc(sizeof *lvl);
    lvl->offset = addr;
    
    struct Stream *stream = stream_open(addr);
    advance(4 * 4);
    strcpy(lvl->timestamp, readstring(0x18));
    advance(0xF0);
    lvl->texture_count = read32();
    
    uint32_t total_texture_count = lvl->texture_count - fix->texture_count;
    advance(total_texture_count * 4); /* Skip texture pointers */
    advance(total_texture_count * 4); /* Skip texture pointers */
    
    lvl->actual_world = readpointer();
    lvl->dynamic_world = readpointer();
    lvl->inactive_dynamic_world = readpointer();
    lvl->father_sector = readpointer();
    lvl->submap_position = readpointer();
    lvl->num_always = read32();
    
    advance(4 * 3); /* Spawnable persos */
    advance(4 * 3);
    
    for (int i = 0; i < 3; i++)
    {
        uint32_t checkpoint = stream->position;
        pointer name_first = readpointer();
        pointer name_last = readpointer();
        uint32_t n_names = read32();
        
        info(COLOR_GREEN "\t[%s names]\n", (const char* []){"Family", "Model", "Instance"}[i]);
        
        stream_seek(stream, name_first);
        for (int n = 0; n < n_names; n++)
        {
            pointer name_next = readpointer();
            pointer name_prev = readpointer();
            pointer header_offset = read32();
            pointer name_offset = readpointer();
            
            const char* cur;
            stream_seek(stream, name_offset);
            switch (i)
            {
                case 0: cur = lvl->family_names[n] = readstringN(); family_color_table[n] = (rand() % 0xFFFFFF) | 0xFF000000; break;
                case 1: cur = lvl->model_names[n] = readstringN(); break;
                case 2: cur = lvl->instance_names[n] = readstringN(); break;
                default: cur =  lvl->instance_names[n] = "Invalid name";
            }
            
            info(COLOR_PINK "\t%s\n", cur);
            
            if (name_next == 0x00) break;
            stream_seek(stream, name_next);
        }
        stream_seek(stream, checkpoint + 3 * 4);
    }
    
    stream_close(stream);
    
    return lvl;
}
