//
//  stdgame.c
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#include "stdgame.h"
#include "global.h"
#include "lvl.h"

STDGAME struct StandardGameInfo *stdgame_read(const address addr)
{
    struct StandardGameInfo *stdgame = malloc(sizeof *stdgame);
    stdgame->offset = addr;
    
    struct Stream *stream = stream_open(addr);
    stdgame->type.family = read32();
    stdgame->type.model = read32();
    stdgame->type.instance = read32();
    stdgame->superobject = readpointer();
    
    advance(0x10);
    
    stdgame->custom_bits.current = read32();
    stdgame->custom_bits_ai.current = read32();
    stdgame->flags = read8();
    stdgame->transparency_zone_min = read32();
    stdgame->transparency_zone_max = read32();
    stdgame->custom_bits.initial = read32();
    stdgame->custom_bits_ai.initial = read32();
    stream_close(stream);
    
    return stdgame;
}

STDGAME const char* stdgame_family_name(struct StandardGameInfo *info)
{
    return lvl->family_names[info->type.family];
}

STDGAME const char* stdgame_model_name(struct StandardGameInfo *info)
{
    return lvl->model_names[info->type.model];
}

STDGAME const char* stdgame_instance_name(struct StandardGameInfo *info)
{
    return lvl->instance_names[info->type.instance];
}

STDGAME void stdgame_free(struct StandardGameInfo *info)
{
    free(info);
}
