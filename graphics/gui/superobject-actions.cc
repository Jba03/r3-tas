static void CarryObject(const struct superobject* object)
{
    uint32* carry = (uint32*)actor_dsgvar(actor_rayman, 167);
    *carry =  game_byteorder_32((offset(object) | 0x80000000));
        
    const struct brain *brain = actor_brain(actor_rayman);
    const struct mind* mind = (const struct mind*)pointer(brain->mind);
    struct intelligence* intell = (struct intelligence*)pointer(mind->intelligence);
    struct intelligence* reflex = (struct intelligence*)pointer(mind->reflex);
    intell->current_behavior = game_byteorder_32(0x80BF2574);
    reflex->current_behavior = game_byteorder_32(0x80C25104);
}

static void display_superobject_actions(const struct superobject* object)
{
    
}
