//
//  engine.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-22.
//

#include "stEngineStructure.h"
#include "game.h"

/** fnEngineLoadLevel: load level with specified name */
void fnEngineLoadLevel(tdstEngineStructure* engine, const char* levelName)
{
    memset((void*)engine->next_level_name, 0, 30);
    strcpy((void*)engine->next_level_name, levelName);
    engine->mode = 6; /* Change map */
}
