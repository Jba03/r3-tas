//
//  intfun.c
//  r3-tas
//
//  Created by Jba03 on 2023-01-01.
//

#include "intfun.h"
#include "game.h"

#include "log.h"

#include "brain.h"
#include "mind.h"
#include "intelligence.h"
#include "dsg.h"

#pragma mark - DSG

// Read from DSG memory of actor `actor` the variable `var` of type `type`, the result being stored in `result`.
static void ReadDSG(struct intcpa* intr, pointer actor_address, unsigned var, uint32_t* type, void** result)
{
    const struct actor* actor = pointer(actor_address);
    if (!actor) goto fail;

    const struct brain* brain = actor_brain(actor);
    if (!brain) goto fail;
    
    const struct mind* mind = pointer(brain->mind);
    if (!mind) goto fail;
    
    const struct dsgmem* dsgmem = pointer(mind->dsgmemory);
    if (!dsgmem) goto fail;
    
    const struct dsgvar* dsgvars = doublepointer(dsgmem->dsgvars);
    if (!dsgvars) goto fail;
    
    if (var >= dsgvars->info_length) goto fail;
    
    const struct dsgvar_info* variable = (struct dsgvar_info*)pointer(dsgvars->info) + var;
    *type = host_byteorder_32(variable->type);
    
    const uint8_t* data = (uint8_t*)pointer(dsgmem->buffer_current);
    if (data)
    {
        *result = (void*)(data + host_byteorder_32(variable->mem_offset));
    }
    
    if (*type <= n_dsgvar_types)
    {
        const char* name = actor_name(actor_instance_name, actor);
        info(BOLD "Read dsgvar %s_%d @ %X (%d) from actor %s\n", dsgvar_typenames[*type], var, offset(*result), host_byteorder_32(*(uint32_t*)*result), name);
    }
        
    return;
    
fail:
    *result = NULL;
}

static void WriteDSG()
{
    
}

void register_dsg_functions(struct intcpa* intr)
{
    intr->dsg.Read = ReadDSG;
    intr->dsg.Write = WriteDSG;
}
