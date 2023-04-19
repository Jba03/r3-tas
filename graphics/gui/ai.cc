#include "stIntelligence.h"
#include "stAIModel.h"
#include "stBehavior.h"
#include "stInputStructure.h"

extern "C"
{
#include "intfun.h"
#include "stActionTable.h"
#include "stTreeInterpret.h"
}

static int line = 1;

static int selected_type;
static int selected_index;
static bool scroll = true;

tdstEngineObject* current_actor;
static tdstIntelligence* current_intelligence_struct = NULL;
static tdstBehavior* current_intelligence = NULL;
static tdstBehavior* current_reflex = NULL;
static tdstMacro* current_macro = NULL;
static tdstNodeInterpret* action_tree = NULL;

static int aimodel_selected_type = 0;
static void* aimodel_selected_data = NULL;

ScriptWindow scriptWindow;

static void display_translated_script(tdstNodeInterpret* tree, bool nodes = false, int pc = -1)
{
    if (ImGui::Button("Load into interpreter"))
    {
        debuggerWindow.load(tree);
        debuggerWindow.setOwner(current_actor);
        debuggerWindow.show = true;
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Replace with binary"))
    {
        std::string path = get_config_path();
        printf("path: %s\n", path.c_str());
        path += "/tree.bin";
        
        FILE* fp = fopen(path.c_str(), "rb");
        if (fp)
        {
            fseek(fp, 0, SEEK_END);
            long sz = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            
            void* buf = malloc(sz);
            fread(buf, sz, 1, fp);
            
            tdstNodeInterpret* node = (tdstNodeInterpret*)buf;
            unsigned length = fnTreeGetLength(node);
            printf("len: %d\n", length);
            memcpy(tree, node, length * sizeof(*node));
            
            fclose(fp);
        }
    }
    
    scriptWindow.tree = tree;
    scriptWindow.display();
}

static void display_aimodel(tdstEngineObject* actor,
                            tdstAIModel* aimodel,
                            tdstBehavior* current_intelligence = NULL,
                            tdstBehavior* current_reflex = NULL,
                            tdstMacro* current_macro = NULL,
                            int* out_type = NULL,
                            void** out_data = NULL)
{
    tdstScriptAI* intelligence_list;
    tdstScriptAI* reflex_list;
    tdstMacroList* macro_list;
    
    ImGui::BeginChild("AILists", ImVec2(max(200, ImGui::GetContentRegionAvail().x / 5), 0), false);
    {
        #pragma mark DSG display
        ImGui::BeginChild("DSG Variables", ImVec2(0, ImGui::GetContentRegionAvail().y / 2), true);
        {
            display_actor_dsg(actor);
        }
        ImGui::EndChild();
        
        #pragma mark Intelligence list
        if ((intelligence_list = (tdstScriptAI*)pointer(aimodel->intelligenceBehaviorList)))
        {
            ImGui::BeginChild("Intelligence", ImVec2(0, ImGui::GetContentRegionAvail().y / 3), true);
            {
                ImGui::Text("Intelligence");
                for (unsigned int i = 0; i < host_byteorder_32(intelligence_list->numBehaviors); i++)
                {
                    tdstBehavior* intelligence = (tdstBehavior*)pointer(intelligence_list->behavior) + i;
                    if (intelligence)
                    {
                        ImVec4 col = intelligence == current_intelligence ? ImVec4(0, 1, 0, 1.0f) : ImVec4(1, 1, 1, 0.5f);
                        ImGui::PushStyleColor(ImGuiCol_Text, col);
                        
                        const char* name = strchr(intelligence->name, ':') + 1;
                        bool selected = selected_type == 0 && selected_index == i;
                        if (ImGui::Selectable(name, &selected))
                        {
                            selected_type = 0;
                            selected_index = i;
                            if (out_data) *out_data = intelligence;
                        }
                        
                        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                        {
                            memory_viewer.GotoAddr = offset(intelligence);
                        }
                        
                        ImGui::PopStyleColor();
                        
                        if (intelligence == current_intelligence && scroll)
                        {
                            ImGui::ScrollToItem();
                            selected_type = 0;
                            selected_index = i;
                            if (out_data) *out_data = intelligence;
                        }
                            
//                        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
//                        {
//                            tdstBrain* br = (tdstBrain*)actor_brain(actor);
//                            tdstMind* mind = (tdstMind*)pointer(br->mind);
//                            tdstIntelligence* intel = (tdstIntelligence*)pointer(mind->intelligence);
//                            intel->current_behavior = intelligence_list->behavior;
//                        }
                    }
                }
            }
            ImGui::EndChild();
        }
        
        #pragma mark Reflex list
        if ((reflex_list = (tdstScriptAI*)pointer(aimodel->reflexBehaviorList)))
        {
            ImGui::BeginChild("Reflex", ImVec2(0, ImGui::GetContentRegionAvail().y / 3), true);
            {
                ImGui::Text("Reflex");
                for (unsigned int r = 0; r < host_byteorder_32(reflex_list->numBehaviors); r++)
                {
                    tdstBehavior* reflex = (tdstBehavior*)pointer(reflex_list->behavior) + r;
                    if (reflex)
                    {
                        ImVec4 col = reflex == current_reflex ? ImVec4(0, 1, 0, 1.0f) : ImVec4(1, 1, 1, 0.5f);
                        ImGui::PushStyleColor(ImGuiCol_Text, col);
                        
                        const char* name = strchr(reflex->name, ':') + 1;
                        bool selected = selected_type == 1 && selected_index == r;
                        if (ImGui::Selectable(name, &selected))
                        {
                            selected_type = 1;
                            selected_index = r;
                            if (out_data) *out_data = reflex;
                        }
                        
                        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                        {
                            memory_viewer.GotoAddr = offset(reflex);
                        }
                        
//                        if (reflex == current_reflex && scroll)
//                        {
//                            ImGui::ScrollToItem();
//                            selected_type = 1;
//                            selected_index = r;
//                            if (out_data) *out_data = reflex;
//                        }
                            
                        ImGui::PopStyleColor();
                    }
                }
            }
            ImGui::EndChild();
        }
        
        #pragma mark Macro list
        macro_list = (tdstMacroList*)pointer(aimodel->macroList);
        if (macro_list && pointer(macro_list->macros))
        {
            ImGui::BeginChild("Macros", ImVec2(0, ImGui::GetContentRegionAvail().y - 30), true);
            {
                ImGui::Text("Macros");
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.5f));
                for (unsigned m = 0; m < macro_list->n_macros; m++)
                {
                    tdstMacro* macro = ((tdstMacro*)pointer(macro_list->macros)) + m;
                    if (macro && macro->script_initial)
                    {
                        const char* name = strchr(macro->name, ':') + 1;
                        
                                    ImVec4 col = ImVec4(1,1,1,0.5f);
//                                    if (interpreter)
//                                        if (interpreter->macro)
//                                            if (strcmp(macro->name, interpreter->macro->name) == 0) col = ImVec4(0, 1.0f, 0.0f, 1.0f);
                                    //const char* name = ImStrchrRange(macro->name, macro->name + 0x100, ':') + 1;
                        bool selected = selected_type == 2 && (selected_index == m || macro == current_macro);
                        if (name)
                        {
                            if (ImGui::Selectable(name, &selected))
                            {
                                selected_type = 2;
                                selected_index = m;
                                if (out_data) *out_data = macro;
                            }
                        }
                                    //if ((frame % 10) == 0) ImGui::ScrollToItem();
                    }
                }
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }
        
        ImGui::Checkbox("Scroll to current", &scroll);
    }
    ImGui::EndChild();
    
    if (out_type) *out_type = selected_type;
}

static void display_ai(tdstEngineObject* actor)
{
    tdstBrain* brain;
    tdstMind* mind;
    tdstAIModel* aimodel;
    tdstIntelligence* intelligence;
    tdstIntelligence* reflex;
    tdstTreeInterpret* script;
    
    current_actor = actor;
    
    static int pc = -1;
    
    if ((brain = (tdstBrain*)actor_brain(actor)))
    {
        current_actor = actor;
        
        if ((mind = (tdstMind*)pointer(brain->mind)))
        {
            if ((intelligence = (tdstIntelligence*)pointer(mind->intelligence)))
            {
                current_intelligence_struct = intelligence;
                current_intelligence = (tdstBehavior*)pointer(intelligence->currentBehavior);
            }
            
            if ((reflex = (tdstIntelligence*)pointer(mind->reflex)))
            {
                current_reflex = (tdstBehavior*)pointer(reflex->currentBehavior);
            }
            
            if ((aimodel = (tdstAIModel*)pointer(mind->aiModel)))
            {
                display_aimodel(actor, aimodel, current_intelligence, current_reflex, current_macro, &aimodel_selected_type, &aimodel_selected_data);
            }
            
            ImGui::SameLine();
            
            
            ImGui::BeginChild("Debugger", ImVec2(ImGui::GetContentRegionAvail().x, 0), false);
            {
//                ImGui::BeginChild("Debug panel", ImVec2(ImGui::GetContentRegionAvail().x, 100), true);
//                {
//                    ImGui::BeginChild("Debug tools", ImVec2(ImGui::GetContentRegionAvail().x / 3, 100), false);
//                    {
//                        const char* intelname = current_intelligence ? strchr(current_intelligence->name, ':') + 1 : NULL;
//                        const char* reflexname = current_reflex ? strchr(current_reflex->name, ':') + 1 : NULL;
//                        
//                        if (intelname) ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), "Intelligence: %s", intelname);
//                        if (reflexname) ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), "Reflex: %s", reflexname);
//                        
//                        if (!interpreter)
//                        {
//                            if (ImGui::Button("Break"))
//                            {
//                                extern void (*cpu_break)(void);
//                                extern bool main_render;
//                                
//                                cpu_break();
//                                main_render = false;
//                                
//                                struct intcpa_param param;
//                                param.actor_self = (actor_superobject(actor)->data);
//                                param.mirror = 1;
//                                param.nowrite = 0;
//                                
//                                interpreter = intcpa_interpreter_create(&param, (tdstTreeInterpret*)pointer(current_intelligence->scripts));
//                                
//                                register_dsg_functions(interpreter);
//                            }
//                        }
//                        else
//                        {
//                            if (ImGui::Button("Step"))
//                            {
//                                intcpa_interpreter_step(interpreter);
//                                pc = intcpa_interpreter_pc(interpreter);
////                                printf("pc: %d\n", pc);
//                            }
//                            
//                            ImGui::SameLine();
//                            if (ImGui::Button("Animate"))
//                            {
//                                animating = true;
//                            }
//                        }
//                        
//                        if (animating && interpreter)
//                        {
//                            intcpa_interpreter_step(interpreter);
//                            pc = intcpa_interpreter_pc(interpreter);
////                            printf("pc: %d\n", pc);
//                        }
//                        
//                        
//                        if (ImGui::Button("Add program breakpoint"))
//                        {
//                            
//                        }
//                        
//                        ImGui::SameLine();
//                        if (ImGui::Button("Add global file breakpoint"))
//                        {
//                            
//                        }
//                    }
//                    ImGui::EndChild();
//                    
//                    ImGui::SameLine();
//                    ImGui::BeginChild("Breakpoints", ImVec2(ImGui::GetContentRegionAvail().x, 100), false);
//                    {
//                        ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), "Macro callstack");
//                        
//                        if (interpreter)
//                        {
//                            for (unsigned int i = interpreter->rsp / 4; i > 1; i--)
//                            {
//                                tdstMacro* m = (tdstMacro*)interpreter->rstack[i * 4 - 2];
//                                //printf("name: %s\n", m->name);
//                                ImGui::TextColored(ImVec4(1,1,1,0.5f), "%s", m->name);
//                            }
//                        }
//                    }
//                    ImGui::EndChild();
//                    
//                    ImGui::SameLine();
//                    ImGui::BeginChild("Macro callstack", ImVec2(ImGui::GetContentRegionAvail().x, 100), false);
//                    {
//                        //ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), "Macro callstack");
//                        
//                    }
//                    ImGui::EndChild();
//                }
//                ImGui::EndChild();
                
                ImGui::Checkbox("View node tree", &scriptWindow.displayNodeTree);
                ImGui::BeginChild("Behavior", ImVec2(ImGui::GetContentRegionAvail().x, 0), true);
                {
                    if (aimodel_selected_data)
                    {
                        if (aimodel_selected_type <= 1) // Intelligence & REFLEX
                        {
                            tdstBehavior* b = (tdstBehavior*)aimodel_selected_data;
                            if (b)
                            {
                                const char* name = strchr(b->name, ':') + 1;
                                const char* typen = aimodel_selected_type ? "REFLEX" : "INTELLIGENCE";
                                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4), "/* %s: %s */", typen, name);
                                ImGui::NewLine();
                                
                                tdstTreeInterpret* script = (tdstTreeInterpret*)pointer(b->scripts);
                                if (script)
                                {
                                    for (unsigned int i = 0; i < b->numScripts; i++)
                                    {
                                        tdstNodeInterpret* tree = (tdstNodeInterpret*)pointer((script + i)->tree);
                                        if (tree)
                                        {
                                            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4), "// Script %d @ %X (%d)", i, offset(tree), fnTreeGetLength(tree));
                                            //printf("script° tree: %X\n", offset(tree));
                                            if (action_tree)
                                            {
                                                //printf("actoin tree: %X\n", offset(action_tree));
                                                display_translated_script(action_tree);
                                            }
                                            else display_translated_script(tree);
                                            ImGui::NewLine();
                                            ImGui::NewLine();
                                        }
                                    }
                                }
                            }
                        }
                        else if (aimodel_selected_type == 2) // Macro
                        {
                            tdstMacro* macro = (tdstMacro*)aimodel_selected_data;
                            tdstTreeInterpret* initial = (tdstTreeInterpret*)pointer(macro->script_initial);
                            if (initial)
                            {
                                const char* name = strchr(macro->name, ':') + 1;
                                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4), "/* MACRO: %s */", name);
                                ImGui::NewLine();
                                
                                tdstNodeInterpret* tree = (tdstNodeInterpret*)pointer(initial->tree);
                                if (tree) display_translated_script(tree);
                            }
                        }
                    }
                }
                ImGui::EndChild();
                
            }
            ImGui::EndChild();
            
//
////                if (interpreter && (frame % 1) == 0)
////                {
////                    intcpa_interpreter_step(interpreter);
////                    pc = intcpa_interpreter_pc(interpreter);
////                }
//
////                if (ImGui::Button("Step"))
////                {
//////                                    IntCPA_Step(&interpreter);
//////                                    pc = IntCPA_GetPC(&interpreter);
////
////                    extern void (*cpu_break)(void);
////                    extern bool main_render;
////
////                    intcpa_interpreter_step(interpreter);
////                    pc = intcpa_interpreter_pc(interpreter);
////                    printf("pc: %d\n", pc);
////
////                    cpu_break();
////                    main_render = false;
////                }
////
//                if ((intelligence = (tdstIntelligence*)pointer(mind->intelligence)))
//                {
//                    if ((behavior = (tdstBehavior*)pointer(intelligence->current_behavior))) {
//
//                        const char* scriptname = strchr(behavior->name, ':') + 1;
////                        if (interpreter)
////                        {
////                            if (interpreter->macro) scriptname = interpreter->macro->name;
////
////                            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4), "/* %s */", scriptname);
////                            ImGui::NewLine();
////                        }
//
//                        for (int i = 0; i < behavior->n_scripts; i++) {
//
//                            if ((script = (tdstTreeInterpret*)pointer(behavior->scripts) + i)) {
//
////                                if (just_selected)
////                                {
////                                    struct intcpa_param param;
////                                    param.mirror = 1;
////                                    param.nowrite = 0;
////                                    param.actor_self = selected_superobject->data;
////
////                                    interpreter = intcpa_interpreter_create(&param, (tdstTreeInterpret*)pointer(behavior->scripts));
////
////                                    register_dsg_functions(interpreter);
////
////
////
////
//////                                                    IntCPA_Init(&interpreter);
//////                                                    tdstNodeInterpret* tr = (tdstNodeInterpret*)pointer(script->tree);
//////
//////                                                    while (!IsEndOfTree(tr))
//////                                                    {
//////                                                        IntCPA_AddNode(&interpreter, tr->type, host_byteorder_32(tr->param), tr->depth);
//////                                                        tr++;
//////                                                    }
//////                                                    IntCPA_AddNode(&interpreter, tr->type, host_byteorder_32(tr->param), tr->depth); /* And EndTree */
//////
//////                                                    IntCPA_BuildTree(&interpreter);
//////
//////                                                    printf("Successfully built interpreter tree.\n");
////
////                                    just_selected = false;
////                                }
////
////                                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4), "// Script %d @ %X (%d)", i, offset(pointer(script->tree)), tree_length((tdstNodeInterpret*)pointer(script->tree)));
////
////
////                                //memory_viewer.GotoAddrAndHighlight(offset(tree), offset(tree));
////
////
////                                                                            int i = 0;
////                                                                            while (1)
////                                                                            {
////                                                                                tdstNodeInterpret node = *(interpreter->tree + i);
////
////                                                                                ImVec4 color = intcpa_interpreter_pc(interpreter) == i ? ImVec4(0,1,0,1) : ImVec4(1,1,1,1);
////
////                                                                                for (int c = 0; c < (node.depth)*2; c++) { ImGui::Text(" "); ImGui::SameLine(); }
////                                                                                ImGui::TextColored(color, "%s (%d)\n", script_nodetype_table[node.type], node.depth);
////
////                                                                                if (node.depth == 0) break;
////                                                                                i++;
////                                                                            }
////
////
////                            }
//
//                            ImGui::NewLine();
//                            ImGui::NewLine();
//                        }
//                    }
//                }
                
                //ImGui::EndChild();
        }
    }
}
