//
//  engine.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-22.
//

#include "stEngineStructure.hh"
#include "game.hh"

/** fnEngineLoadLevel: load level with specified name */
void fnEngineLoadLevel(tdstEngineStructure* engine, const char* levelName)
{
    memset((void*)engine->nextLevelName, 0, 30);
    strcpy((void*)engine->nextLevelName, levelName);
    engine->mode = 6; /* Change map */
}
