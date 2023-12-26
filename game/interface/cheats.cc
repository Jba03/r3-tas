//
//  cheats.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-15.
//

#include "cheats.hh"
#include "memory.hh"
#include "stBrain.hh"
#include "stMind.hh"
#include "stIntelligence.hh"

static void begin_carrying_object(const tdstSuperObject* carryable_actor)
{
    uint32_t* dsgVar167 = fnActorGetDsgVar(actor_rayman, 167, NULL); /* DsgVar 167: currently carried actor superobject */
    if (!dsgVar167) return;
    
    *dsgVar167 = game_address(offset(carryable_actor));
    
    const tdstBrain *brain = actor_brain(actor_rayman);
    if (!brain) return;
    
    const tdstMind* mind = (const tdstMind*)pointer(brain->mind);
    if (!mind) return;
    
    tdstIntelligence* intell = (tdstIntelligence*)pointer(mind->intelligence);
    if (intell) intell->currentBehavior = game_byteorder_32(0x80BF2574);
    
    tdstIntelligence* reflex = (tdstIntelligence*)pointer(mind->reflex);
    reflex->currentBehavior = game_byteorder_32(0x80C25104);
}

const struct cheats cheats =
{
    &begin_carrying_object,
};
