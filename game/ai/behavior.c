//
//  behavior.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#include "behavior.h"

#include <sys/stat.h>

BEHAVIOR struct Behavior* behavior_read(const address addr)
{
    struct Behavior* behavior = malloc(sizeof *behavior);
    behavior->offset = addr;
    
    struct Stream* stream = stream_open(addr);
    behavior->name = readstring(0x100);
    behavior->ptr_scripts = readpointer();
    behavior->ptr_script_first = readpointer();
    behavior->n_scripts = read8();
    
    char dir[1024];
    memset(dir, 0, 1024);
    sprintf(dir, LIBR3TAS_DIR "/behavior");
    
    mkdir(dir, 0755);
    
    memset(dir, 0, 1024);
    sprintf(dir, LIBR3TAS_DIR "/translated");
    
    mkdir(dir, 0755);
    
    char path[1024];
    memset(path, 0, 1024);
    sprintf(path, LIBR3TAS_DIR "/behavior/%s.cpa", behavior->name);
    
    char translation_path[1024];
    memset(translation_path, 0, 1024);
    sprintf(translation_path, LIBR3TAS_DIR "/translated/%s.scpt", behavior->name);
    
    FILE* fp = fopen(path, "wb");
    FILE* fp_translated = fopen(translation_path, "wb");
    
    behavior->scripts = malloc(sizeof(struct Script*) * behavior->n_scripts);
    if (behavior->ptr_scripts != 0x00 && behavior->n_scripts > 0)
    {
        for (unsigned i = 0; i < behavior->n_scripts; i++)
        {
            behavior->scripts[i] = script_read(behavior->ptr_scripts + 4 * i);
            
            if (behavior->scripts[i]->nodes)
            {
                for (int s = 0; s < array_element_count(behavior->scripts[i]->nodes); s++)
                {
                    struct ScriptNode* node = array_get(behavior->scripts[i]->nodes, s);
                    fwrite(&node->type, sizeof(uint32_t), 1, fp);
                    fwrite(&node->param, sizeof(uint32_t), 1, fp);
                    fwrite(&node->depth, sizeof(uint8_t), 1, fp);
                }
            }
            
            const char* translation = script_translate(behavior->scripts[i]);
            //fwrite("\n// Script ", 1, strlen("\n// New script"), fp_translated);
            fprintf(fp_translated, "\n// Script %d\n", i);
            fwrite(translation, sizeof(const char), strlen(translation), fp_translated);
        }
    }
        
    fclose(fp);
    fclose(fp_translated);
    //behavior->scripts[0] = script_read(behavior->ptr_script_first);
    
    stream_close(stream);
    
    
    
    return behavior;
}
