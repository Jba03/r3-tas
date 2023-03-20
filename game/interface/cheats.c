//
//  cheats.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-15.
//

#include "cheats.h"
#include "stBrain.h"
#include "stMind.h"
#include "stIntelligence.h"
// 10a4214
// 80f95580
// 80f742f4
static void begin_carrying_object(const tdstSuperObject* carryable_actor)
{
    uint32_t* dsgvar_167; /* DsgVar 167: currently carried actor superobject */
    if (actor_dsgvar(actor_rayman, 167, NULL, (void**)&dsgvar_167) < 0) return;
    *dsgvar_167 = game_address(offset(carryable_actor));
    
    const tdstBrain *brain = actor_brain(actor_rayman);
    if (!brain) return;
    
    const tdstMind* mind = (const tdstMind*)pointer(brain->mind);
    if (!mind) return;
    
    tdstIntelligence* intell = (tdstIntelligence*)pointer(mind->intelligence);
    if (intell) intell->current_behavior = game_byteorder_32(0x80BF2574);
    
    tdstIntelligence* reflex = (tdstIntelligence*)pointer(mind->reflex);
    reflex->current_behavior = game_byteorder_32(0x80C25104);
}

const struct cheats cheats =
{
    &begin_carrying_object,
};
