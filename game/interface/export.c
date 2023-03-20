//
//  export.c
//  r3-tas
//
//  Created by Jba03 on 2023-01-08.
//

#include "export.h"

#include "stEngineObject.h"
#include "stBrain.h"
#include "stMind.h"
#include "stAIModel.h"
#include "stBehavior.h"
#include "stTreeInterpret.h"
#include "stInputStructure.h"

#include <stdio.h>
#include <stdlib.h>

#include "translate.h"

extern const char* (*get_config_path)(void);

static void export_behavior(tdstBehavior* behavior)
{
    char path[4096];
    memset(path, 0, 4096);
    
    sprintf(path, "%s/libr3/behavior/%s.scpt", get_config_path(), behavior->name);

    FILE* fp = fopen(path, "w");
    
    fprintf(fp, "/* %s */\n\n", behavior->name);
    
    for (int i = 0; i < behavior->n_scripts; i++)
    {
        tdstTreeInterpret* script = (tdstTreeInterpret*)pointer(behavior->scripts) + i;
        tdstNodeInterpret* tree = pointer(script->tree);
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
                        
                        
                        tdstEngineObject* actor = (tdstEngineObject*)pointer(tok.node->param);
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
                        tdstInputEntryElement* entry = (tdstInputEntryElement*)pointer(tok.node->param);
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
            
            script_translation_free(translation);
        }
        
        
    }
    
    fclose(fp);
}

void export_scripts(void)
{
    tdstSuperObject* root = superobject_first_child(hierarchy);
    superobject_for_each_type(superobject_type_actor, root, object)
    {
        const tdstEngineObject* actor = (const tdstEngineObject*)superobject_data(object);
        if (!actor) continue;
        
        const tdstBrain* brain = (const tdstBrain*)pointer(actor->brain);
        if (!brain) continue;
        
        const tdstMind* mind = (const tdstMind*)pointer(brain->mind);
        if (!mind) continue;
        
        const tdstAIModel* aimodel = (const tdstAIModel*)pointer(mind->ai_model);
        if (!aimodel) continue;
        
        const tdstScriptAI* intelligence_list = (const tdstScriptAI*)pointer(aimodel->intelligence_behavior_list);
        const tdstScriptAI* reflex_list = (const tdstScriptAI*)pointer(aimodel->reflex_behavior_list);
        const tdstMacroList* macro_list = (const tdstMacroList*)pointer(aimodel->macrolist);
        
        /* Get actor instance name, or model name if spawnable actor */
        const char* name = actor_name(actor_instance_name, actor);
        if (!name) name = actor_name(actor_model_name, actor);
        
        
        if (intelligence_list)
        {
            for (int i = 0; i < host_byteorder_32(intelligence_list->n_behaviors); i++)
            {
                tdstBehavior* behavior = (tdstBehavior*)pointer(intelligence_list->behavior) + i;
                
                if (behavior) export_behavior(behavior);
            }
        }
        
        
    };
}
