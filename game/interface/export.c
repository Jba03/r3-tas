//
//  export.c
//  r3-tas
//
//  Created by Jba03 on 2023-01-08.
//

#include <stdio.h>
#include <stdlib.h>

#include "export.h"
#include "memory.h"

#include "stEngineObject.h"
#include "stBrain.h"
#include "stMind.h"
#include "stAIModel.h"
#include "stBehavior.h"
#include "stTreeInterpret.h"
#include "stInputStructure.h"

extern const char* (*get_config_path)(void);

static void export_behavior(tdstBehavior* behavior)
{
    char path[4096];
    memset(path, 0, 4096);
    
    sprintf(path, "%s/libr3/behavior/%s.scpt", get_config_path(), behavior->name);

    FILE* fp = fopen(path, "w");
    
    fprintf(fp, "/* %s */\n\n", behavior->name);
    
    for (int i = 0; i < behavior->numScripts; i++)
    {
        tdstTreeInterpret* script = (tdstTreeInterpret*)pointer(behavior->scripts) + i;
        tdstNodeInterpret* tree = pointer(script->tree);
        if (!tree) continue;
        
        tdstTreeTranslationContext* c = NULL;
        if (fnTreeTranslate(&c, tree, NULL))
        {
            fprintf(fp, "// Script %d @ %X\n", i, offset(tree));
            
            tdstTreeTranslationToken *tok = c->token;
            while (tok)
            {
                if (tok->originalNode)
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
                    
                    if (tok->originalNode->type == script_node_type_actorref)
                    {
                        
                        
                        tdstEngineObject* actor = (tdstEngineObject*)pointer(tok->originalNode->param);
                        if (actor) {
                            fprintf(fp, "\"%s\"", fnActorGetName(actor_instance_name, actor, objectType));
                        }
                        continue;
                    }
                    
                    if (tok->originalNode->type == script_node_type_behaviorref)
                    {
                        const char* name = (const char*)pointer(tok->originalNode->param);
                        const char* shortname = strchr(name, ':') + 1;
                        fprintf(fp, "\"%s\"", shortname);
                        continue;
                    }
                    
                    if (tok->originalNode->type == script_node_type_button)
                    {
                        tdstInputEntryElement* entry = (tdstInputEntryElement*)pointer(tok->originalNode->param);
                        const char* name = (const char*)pointer(entry->actionName);
                        fprintf(fp, "\"%s\"", name);
                        continue;
                    }
                    
                    if (tok->originalNode->type == script_node_type_actionref)
                    {
                        const char* name = (const char*)pointer(tok->originalNode->param);
                        fprintf(fp, "\"%s\"", name);
                        continue;
                    }
                    
                    if (tok->originalNode->type == script_node_type_subroutine)
                    {
                        const char* name = (const char*)pointer(tok->originalNode->param);
                        const char* shortname = strchr(name, ':') + 1;
                        fprintf(fp, "%s", shortname);
                        continue;
                    }
                }
                //
                fprintf(fp, "%s", tok->translatedText);
                
                //if (tok.string[0] == '\n') linenn++;
                //if (tok.string[0] != '\n') ImGui::SameLine();
                //ImGui::PopStyleVar();
            }
            
            fprintf(fp, "\n\n");
            
            //script_translation_free(translation);
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
        
        const tdstAIModel* aimodel = (const tdstAIModel*)pointer(mind->aiModel);
        if (!aimodel) continue;
        
        const tdstScriptAI* intelligence_list = (const tdstScriptAI*)pointer(aimodel->intelligenceBehaviorList);
        const tdstScriptAI* reflex_list = (const tdstScriptAI*)pointer(aimodel->reflexBehaviorList);
        const tdstMacroList* macro_list = (const tdstMacroList*)pointer(aimodel->macroList);
        
        /* Get actor instance name, or model name if spawnable actor */
        const char* name = fnActorGetName(actor_instance_name, actor, objectType);
        if (!name) name = fnActorGetName(actor_model_name, actor, objectType);
        
        
        if (intelligence_list)
        {
            for (int i = 0; i < host_byteorder_32(intelligence_list->numBehaviors); i++)
            {
                tdstBehavior* behavior = (tdstBehavior*)pointer(intelligence_list->behavior) + i;
                
                if (behavior) export_behavior(behavior);
            }
        }
        
        
    };
}
