//
//  engine.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-22.
//

#include "engine.h"
#include "game.h"

/** engine_load_level: load level with specified name */
void engine_load_level(struct engine* engine, const char* level_name)
{
    memset((void*)engine->next_level_name, 0, 30);
    strcpy((void*)engine->next_level_name, level_name);
    engine->mode = 6; /* Change map */
}
