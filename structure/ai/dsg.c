//
//  dsg.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#include "dsg.h"
#include "stream.h"

#include <stdio.h>
#include <stdlib.h>

DSG struct DSGVariableInfo dsgvar_info_read(const address addr)
{
    struct DSGVariableInfo info;
    info.offset = addr;
    
    struct Stream* stream = stream_open(addr);
    info.i_mem_offset = read32();
    info.type_id = read32();
    info.save_type = read32(); /* advance(2) + read16() ? */
    info.init_type = read32();
    stream_close(stream);
    
    if (info.type_id > N_DSGVAR_TYPES)
        info.type_id = N_DSGVAR_TYPES;
    
    info.type_name = dsgvar_typenames[info.type_id + 1];
    
    info("DSGVar @ %X: %s\n", addr, info.type_name);
    
    return info;
}

DSG struct DSGVariable* dsgvar_read(const address addr)
{
    struct DSGVariable* var = malloc(sizeof *var);
    var->offset = addr;
    var->entries = NULL;
    
    struct Stream* stream = stream_open(addr);
    var->ptr_mem_offset = readpointer();
    var->ptr_info = readpointer();
    var->memory_length = read32();
    var->info_length = read8();
    advance(0x3); /* Align */
    
    if (var->info_length > 0)
    {
        var->entries = malloc(sizeof(struct DSGVariableInfo) * var->info_length);
        
        stream_seek(stream, var->ptr_info);
        for (unsigned i = 0; i < var->info_length; i++)
        {
            var->entries[i] = dsgvar_info_read(stream->position);
            advance(4 * 4);
        }
    }
    
    stream_close(stream);

    return var;
}

DSG struct DSGMemory* dsgmemory_read(const address addr)
{
    struct DSGMemory* mem = malloc(sizeof *mem);
    mem->offset = addr;
    mem->var = NULL;
    
    struct Stream* stream = stream_open(addr);
    mem->ptr_dsgvars = readpointer();
    mem->ptr_memory_initial = readpointer();
    mem->ptr_memory_current = readpointer();
    stream_close(stream);
    
    if (mem->ptr_dsgvars != 0x00)
    {
        pointer dsgvar = memory.read_32(mem->ptr_dsgvars) & 0xFFFFFFF;
        mem->var = dsgvar_read(dsgvar);
        
        if (mem->var->info_length > 0)
        {
            /* More convenient */
            mem->n_variables = mem->var->info_length;
            
            /* Read initial */
            if (mem->ptr_memory_initial != 0x00)
            {
                mem->initial = malloc(sizeof(struct DSGVariableInfo) * mem->n_variables);
                for (unsigned i = 0; i < mem->n_variables; i++)
                {
                    struct DSGVariableInfo entry = mem->var->entries[i];
                    
                    address absolute_offset = mem->ptr_memory_initial + entry.i_mem_offset;
                    entry.data_offset = absolute_offset;
                    
                    mem->initial[i] = entry;
                }
            }
            
            /* Read current */
            if (mem->ptr_memory_current != 0x00)
            {
                mem->current = malloc(sizeof(struct DSGVariableInfo) * mem->n_variables);
                for (unsigned i = 0; i < mem->n_variables; i++)
                {
                    struct DSGVariableInfo entry = mem->var->entries[i];
                    
                    address absolute_offset = mem->ptr_memory_current + entry.i_mem_offset;
                    entry.data_offset = absolute_offset;
                    
                    mem->current[i] = entry;
                }
            }
        }
    }
    
    return mem;
}
