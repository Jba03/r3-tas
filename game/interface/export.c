//
//  export.c
//  r3-tas
//
//  Created by Jba03 on 2023-01-08.
//

#include "export.h"

#include "actor.h"
#include "brain.h"
#include "mind.h"
#include "aimodel.h"
#include "behavior.h"
#include "script.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#include "translate.h"

static void export_behavior(struct behavior* behavior)
{
    char path[4096] = LIBR3TAS_DIR;
    sprintf(path, "%s/behavior/%s.scpt", path, behavior->name);

    FILE* fp = fopen(path, "w");
    
    fprintf(fp, "/* %s */\n\n", behavior->name);
    
    for (int i = 0; i < behavior->n_scripts; i++)
    {
        struct script* script = (struct script*)pointer(behavior->scripts) + i;
        struct script_node* tree = pointer(script->tree);
        if (!tree) continue;
        
        struct translation* translation = script_translate(tree);

        if (translation)
        {
            fprintf(fp, "// Script %d @ %X\n", i, offset(tree));
            
            for (int t = 0; t < translation->n_tokens; t++)
            {
                struct translation_token tok = translation->token[t];
                
                if (tok.node)
                {
                    //                    switch (tok.node->type)
                    //                    {
                    //                        case script_node_type_keyword: color = ImVec4(198.0f / 255.0f, 121.0f / 255.0f, 221.0f / 255.0f, 1.0f); break;
                    //                        case script_node_type_condition: color = ImVec4(0.3, 0.5, 1.0f, 1.0f); break;
                    //                            //case script_node_type_operator: color = ImVec4(170.0f / 255.0f, 13.0f / 255.0f, 145.0f / 255.0f, 1.0f); break;
                    //                        case script_node_type_function: color = ImVec4(0.3, 0.5, 1.0f, 1.0f); break;
                    //                        case script_node_type_procedure: color = ImVec4(0.3, 0.5, 1.0f, 1.0f); break;
                    //                        case script_node_type_dsgvarref:
                    //                        case script_node_type_dsgvarref2: color = ImVec4(0.9, 0.4, 0.45, 1.0f); break;
                    //                        case script_node_type_constant:
                    //                        case script_node_type_real: color = ImVec4(210.0f / 255.0f, 148.0f / 255.0f, 93.0f / 255.0f, 1.0f); break;
                    //                        case script_node_type_constant_vector:
                    //                        case script_node_type_vector: color = ImVec4(229.0f / 255.0f, 193.0f / 255.0f, 124.0f / 255.0f, 1.0f); break;
                    //                        case script_node_type_field: color = ImVec4(170.0f / 255.0f, 13.0f / 255.0f, 145.0f / 255.0f, 1.0f); break;
                    //                        case script_node_type_subroutine: color = ImVec4(84.0f / 255.0f, 222.0f / 255.0f, 101.0f / 255.0f, 1.0f); break;
                    //                    }
                    
                    if (tok.node->type == script_node_type_actorref)
                    {
                        
                        
                        struct actor* actor = (struct actor*)pointer(tok.node->param);
                        if (actor) {
                            fprintf(fp, "\"%s\"", actor_name(actor_instance_name, actor));
                        }
                        continue;
                    }
                    
                    if (tok.node->type == script_node_type_behaviorref)
                    {
                        const char* name = (const char*)pointer(tok.node->param);
                        const char* shortname = strchr(name, ':') + 1;
                        fprintf(fp, "\"%s\"", shortname);
                        continue;
                    }
                    
                    if (tok.node->type == script_node_type_button)
                    {
                        struct input_entry* entry = (struct input_entry*)pointer(tok.node->param);
                        const char* name = (const char*)pointer(entry->action_name);
                        fprintf(fp, "\"%s\"", name);
                        continue;
                    }
                    
                    if (tok.node->type == script_node_type_actionref)
                    {
                        const char* name = (const char*)pointer(tok.node->param);
                        fprintf(fp, "\"%s\"", name);
                        continue;
                    }
                    
                    if (tok.node->type == script_node_type_subroutine)
                    {
                        const char* name = (const char*)pointer(tok.node->param);
                        const char* shortname = strchr(name, ':') + 1;
                        fprintf(fp, "%s", shortname);
                        continue;
                    }
                }
                //
                fprintf(fp, "%s", tok.string);
                
                //if (tok.string[0] == '\n') linenn++;
                //if (tok.string[0] != '\n') ImGui::SameLine();
                //ImGui::PopStyleVar();
            }
            
            fprintf(fp, "\n\n");
            
            translation_destroy(translation);
        }
        
        
    }
    
    fclose(fp);
}

void export_scripts(void)
{
    struct superobject* root = superobject_first_child(hierarchy);
    superobject_for_each_type(superobject_type_actor, root, object)
    {
        const struct actor* actor = (const struct actor*)superobject_data(object);
        if (!actor) continue;
        
        const struct brain* brain = (const struct brain*)pointer(actor->brain);
        if (!brain) continue;
        
        const struct mind* mind = (const struct mind*)pointer(brain->mind);
        if (!mind) continue;
        
        const struct aimodel* aimodel = (const struct aimodel*)pointer(mind->ai_model);
        if (!aimodel) continue;
        
        const struct behavior_list* intelligence_list = (const struct behavior_list*)pointer(aimodel->intelligence_behavior_list);
        const struct behavior_list* reflex_list = (const struct behavior_list*)pointer(aimodel->reflex_behavior_list);
        const struct macro_list* macro_list = (const struct macro_list*)pointer(aimodel->macrolist);
        
        /* Get actor instance name, or model name if spawnable actor */
        const char* name = actor_name(actor_instance_name, actor);
        if (!name) name = actor_name(actor_model_name, actor);
        
        
        if (intelligence_list)
        {
            for (int i = 0; i < host_byteorder_32(intelligence_list->n_behaviors); i++)
            {
                struct behavior* behavior = (struct behavior*)pointer(intelligence_list->behavior) + i;
                
                if (behavior) export_behavior(behavior);
            }
        }
        
        
    };
}
