//
//  intfun.c
//  r3-tas
//
//  Created by Jba03 on 2023-01-01.
//

#include "intfun.h"
#include "game.h"

#include "log.h"

#include "stBrain.h"
#include "stMind.h"
#include "stIntelligence.h"
#include "stDsg.h"

#pragma mark - DSG

// Read from DSG memory of actor `actor` the variable `var` of type `type`, the result being stored in `result`.
static void ReadDSG(struct intcpa* intr, pointer actor_address, unsigned var, uint32_t* type, void** result)
{
    const tdstEngineObject* actor = pointer(actor_address);
    if (!actor) goto fail;

    const tdstBrain* brain = actor_brain(actor);
    if (!brain) goto fail;
    
    const tdstMind* mind = pointer(brain->mind);
    if (!mind) goto fail;
    
    const tdstDsgMem* dsgmem = pointer(mind->dsgMem);
    if (!dsgmem) goto fail;
    
    const tdstDsgVar* dsgvars = doublepointer(dsgmem->dsgVars);
    if (!dsgvars) goto fail;
    
    if (var >= dsgvars->infoLength) goto fail;
    
    const tdstDsgVarInfo* variable = (tdstDsgVarInfo*)pointer(dsgvars->info) + var;
    *type = host_byteorder_32(variable->type);
    
    const uint8_t* data = (uint8_t*)pointer(dsgmem->currentBuffer);
    if (data)
    {
        *result = (void*)(data + host_byteorder_32(variable->memOffset));
    }
    
    if (*type <= n_dsgvar_types)
    {
        const char* name = fnActorGetName(actor_instance_name, actor, objectType);
        info(BOLD "Read dsgvar %s_%d @ %X (%d) from actor %s\n", dsgvar_typenames[*type], var, offset(*result), host_byteorder_32(*(uint32_t*)*result), name);
    }
        
    return;
    
fail:
    *result = NULL;
}

static void WriteDSG()
{
    
}

//void register_dsg_functions(struct intcpa* intr)
//{
//    intr->dsg.Read = ReadDSG;
//    intr->dsg.Write = WriteDSG;
//}
