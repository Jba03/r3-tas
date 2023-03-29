//
//  fnDsg.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-28.
//

#include "game.h"
#include "stDsg.h"

static tdstDsgVar* fnDsgMemGetDsgVars(const tdstDsgMem *mem)
{
    if (!mem) return NULL;
    return doublepointer(mem->dsgVars);
}

static void* fnDsgMemGetInitialBuffer(const tdstDsgMem *mem)
{
    if (!mem) return NULL;
    return pointer(mem->initialBuffer);
}

static void* fnDsgMemGetCurrentBuffer(const tdstDsgMem *mem)
{
    if (!mem) return NULL;
    return pointer(mem->currentBuffer);
}

static tdstDsgVarInfo* fnDsgMemGetDsgVarInfo(const tdstDsgMem *mem, uint8 var)
{
    const tdstDsgVar* variables = fnDsgMemGetDsgVars(mem);
    if (!variables)
        return NULL;
    
    if (var >= variables->infoLength)
        return NULL;
    
    tdstDsgVarInfo *info = (tdstDsgVarInfo*)pointer(variables->info);
    if (!info)
        return NULL;
    
    return info + var;
}

static void *fnDsgMemGetDsgVar(const tdstDsgMem *mem, uint8 var, bool initialMemory, uint8 *type)
{
    const tdstDsgVarInfo *info = fnDsgMemGetDsgVarInfo(mem, var);
    if (!info)
        return NULL;
    
    const uint8 *buffer = initialMemory ? fnDsgMemGetInitialBuffer(mem) : fnDsgMemGetCurrentBuffer(mem);
    if (!buffer)
        return NULL;
    
    if (type) *type = host_byteorder_32(info->type);
    return (void*)(buffer + host_byteorder_32(info->memOffset));
}
