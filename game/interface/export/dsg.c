#include "global.h"
#include "engine.h"

#include <sys/stat.h>

static void export(struct SuperObject* so, void* param)
{
    if (!so) return;
    struct Actor* actor = so->data;
    if (!actor->brain) return;
    struct Brain* brain = actor->brain;
    if (!brain->mind) return;
    struct Mind* mind = brain->mind;
    if (!mind->dsg) return;
    struct DSGMemory* mem = mind->dsg;
    
    char path[1024];
    memset(path, 0, 1024);
    sprintf(path, LIBR3TAS_DIR "/dsgdump/%X.dsg", actor->superobject->offset);
    
    FILE* fp = fopen(path, "wb");
    
    for (int i = 0; i < mem->n_variables; i++)
    {
        struct DSGVariableInfo var = mem->current[i];
        fwrite(&var.offset, sizeof(uint32_t), 1, fp);
        fwrite(&var.type_id, sizeof(uint32_t), 1, fp);
        //fwrite(&var.data_offset, sizeof(uint32_t), 1, fp);
        
        switch (var.type_id)
        {
            case DSGVAR_TYPE_BYTE:
            {
                int8_t value = memory.read_8(var.data_offset);
                fwrite(&value, 1, sizeof(int8_t), fp);
                break;
            }
                
            case DSGVAR_TYPE_UBYTE:
            {
                uint8_t value = memory.read_8(var.data_offset);
                fwrite(&value, 1, sizeof(uint8_t), fp);
                break;
            }
                
            case DSGVAR_TYPE_SHORT:
            {
                int16_t value = memory.read_16(var.data_offset);
                fwrite(&value, 1, sizeof(int16_t), fp);
                break;
            }
                
            case DSGVAR_TYPE_USHORT:
            {
                uint16_t value = memory.read_16(var.data_offset);
                fwrite(&value, 1, sizeof(uint16_t), fp);
                break;
            }
                
            case DSGVAR_TYPE_INT:
            {
                int32_t value = memory.read_32(var.data_offset);
                fwrite(&value, 1, sizeof(int32_t), fp);
                break;
            }
                
            case DSGVAR_TYPE_UINT:
            {
                uint32_t value = memory.read_32(var.data_offset);
                fwrite(&value, 1, sizeof(uint32_t), fp);
                break;
            }
                
            case DSGVAR_TYPE_FLOAT:
            {
                float value = memory.read_float(var.data_offset);
                fwrite(&value, 1, sizeof(float), fp);
                break;
            }
                
            case DSGVAR_TYPE_VECTOR:
            {
                struct vector3 value = struct vector3_read(var.data_offset);
                fwrite(&value.x, 1, sizeof(float), fp);
                fwrite(&value.y, 1, sizeof(float), fp);
                fwrite(&value.z, 1, sizeof(float), fp);
                break;
            }
        }
    }
    
    fclose(fp);
}

void game_export_dsg(void)
{
    char dir[1024];
    memset(dir, 0, 1024);
    sprintf(dir, LIBR3TAS_DIR "/dsgdump");
    
    mkdir(dir, 0755);
    
    superobject_for_each(SUPEROBJECT_TYPE_ACTOR, engine->root, &export, NULL);
}
