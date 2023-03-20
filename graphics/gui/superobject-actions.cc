static void CarryObject(const tdstSuperObject* object)
{
    uint32* carry = (uint32*)actor_dsgvar(actor_rayman, 167);
    *carry =  game_byteorder_32((offset(object) | 0x80000000));
        
    const tdstBrain *brain = actor_brain(actor_rayman);
    const tdstMind* mind = (const tdstMind*)pointer(brain->mind);
    tdstIntelligence* intell = (tdstIntelligence*)pointer(mind->intelligence);
    tdstIntelligence* reflex = (tdstIntelligence*)pointer(mind->reflex);
    intell->current_behavior = game_byteorder_32(0x80BF2574);
    reflex->current_behavior = game_byteorder_32(0x80C25104);
}

static void display_superobject_actions(const tdstSuperObject* object)
{
    
}
