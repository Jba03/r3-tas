static void CarryObject(const stSuperObject* object)
{
    uint32* carry = (uint32*)actor_dsgvar(actor_rayman, 167);
    *carry =  game_byteorder_32((offset(object) | 0x80000000));
        
    const stBrain *brain = actor_brain(actor_rayman);
    const stMind* mind = (const stMind*)pointer(brain->mind);
    stIntelligence* intell = (stIntelligence*)pointer(mind->intelligence);
    stIntelligence* reflex = (stIntelligence*)pointer(mind->reflex);
    intell->current_behavior = game_byteorder_32(0x80BF2574);
    reflex->current_behavior = game_byteorder_32(0x80C25104);
}

static void display_superobject_actions(const stSuperObject* object)
{
    
}
