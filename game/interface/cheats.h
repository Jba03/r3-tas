//
//  cheats.h
//  r3-tas
//
//  Created by Jba03 on 2023-03-15.
//

#ifndef cheats_h
#define cheats_h

#include "game.h"

struct cheats
{
    void (*begin_carrying_object)(const struct superobject* carryable_actor);
};

const extern struct cheats cheats;

#endif /* cheats_h */
