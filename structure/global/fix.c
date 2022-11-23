//
//  fix.c
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#include "fix.h"
#include "logging.h"
#include "stream.h"

FIX_STRUCTURE struct FIX* fix_read(const address addr)
{
    struct FIX* fix = malloc(sizeof *fix);
    fix->offset = addr;
    
    struct Stream *stream = stream_open(addr);
    advance(4 * 4);
    advance(4 * 4);
    
    fix->identity_matrix = readpointer();
    fix->localization_structure = readpointer();
    fix->level_name_count = read32();
    fix->entry_count = read32();
    
    info(COLOR_GREEN "\t[Demo saves]\n");
    for (int i = 0; i < fix->entry_count; i++)
    {
        const char* entry = readstring(0xC);
        info(COLOR_PINK "\t[fix @ %X]: " COLOR_BLUE "%s\n", stream->position, entry);
    }
    
    info(COLOR_GREEN "\t[Demo level names]\n");
    for (int i = 0; i < fix->entry_count; i++)
    {
        const char* entry = readstring(0xC);
        info(COLOR_PINK "\t[fix @ %X]: " COLOR_BLUE "%s\n", stream->position, entry);
    }
    
    info(COLOR_GREEN "\t[Level names]\n");
    for (int i = 0; i < fix->level_name_count; i++)
    {
        const char* entry = readstring(0x1E);
        info(COLOR_PINK "\t[fix @ %X]: " COLOR_BLUE "%s\n", stream->position, entry);
    }
    
    info(COLOR_GREEN "\t[Entry level name]\n");
    const char* entry_level = readstring(0x1E);
    info(COLOR_PINK "\t[fix @ %X]: " COLOR_BLUE "%s\n", stream->position, entry_level);
    
    advance(2);
    
    fix->language_count = read32();
    fix->language_offset = readpointer();
    fix->texture_count = read32();
    
    info(COLOR_GREEN "\t[Texture count: %X]\n\n", fix->texture_count);
    
    stream_close(stream);
    
    return fix;
}
