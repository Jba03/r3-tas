//
//  stdgame.h
//  r3lib
//
//  Created by Jba03 on 2022-11-21.
//

#ifndef stdgame_h
#define stdgame_h

#include "stream.h"
#include "superobject.h"

#define STDGAME

struct StandardGameInfo {
    /* superobject which holds the info */
    pointer superobject;
    
    struct { uint32_t family, model, instance;  } type;
    struct { uint32_t current, initial;         } custom_bits;
    struct { uint32_t current, initial;         } custom_bits_ai;
    
    uint8_t flags;
    uint8_t transparency_zone_min;
    uint8_t transparency_zone_max;
    
    address offset;
};

STDGAME struct StandardGameInfo *stdgame_read(const address addr);
STDGAME const char* stdgame_family_name(struct StandardGameInfo *info);
STDGAME const char* stdgame_model_name(struct StandardGameInfo *info);
STDGAME const char* stdgame_instance_name(struct StandardGameInfo *info);
STDGAME void stdgame_free(struct StandardGameInfo *info);

#endif /* stdgame_h */
