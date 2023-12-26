//
//  superobject.cpp
//  r3-tas
//
//  Created by Jba03 on 2022-12-22.
//

#include "gui.hh"

#include <bitset>
#include <vector>

#include "stInputStructure.hh"
#include "stEngineObject.hh"
#include "stTreeInterpret.hh"
#include "stBrain.hh"
#include "stMind.hh"
#include "stIntelligence.hh"
#include "stBehavior.hh"
#include "stDsg.hh"
#include "stTransform.hh"
#include "stAIModel.hh"
#include "game.hh"

static int window_counter = 0;

struct SuperobjectInfoWindow
{
    bool active;
    bool sidebar;
    int sidebar_idx;
    const stSuperObject* selected;
    
    SuperobjectInfoWindow()
    {
        active = true;
        sidebar = false;
    }
    
    void Draw()
    {
        if (active && hierarchy)
        {
            const char* name = fnSuperobjectGetName(selected);
            if (!name) name = "Invalid object name";
            
            ImGui::Begin(name);
            
            if (true)
            {
                ImGui::BeginChild("SO List", ImVec2(200, 0), true);
                
                int i = 0;
                
                const stSuperObject* world = dynamic_world;
                if (superobject_type(selected) == superobject_type_actor) world = dynamic_world;
                if (superobject_type(selected) == superobject_type_ipo) world = father_sector;
                if (superobject_type(selected) == superobject_type_sector) world = father_sector;
                
                superobject_for_each(world, child)
                {
                    const stEngineObject* actor = (const stEngineObject*)superobject_data(child);
                    if (!actor) continue;
                    
                    /* Get actor instance name, or model name if spawnable actor */
                    const char* name = fnSuperobjectGetName(child);
                    if (!name) name = "Invalid object name";
                    
                    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
                    if (superobject_type(child) == superobject_type_actor) color = ImColor(actor_color(actor));
                    
                    /* Draw the list item */
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    if (ImGui::Selectable(name, sidebar_idx == i))
                    {
                        selected = (stSuperObject*)child;
                        sidebar_idx = i;
                    }
                    
                    ImGui::PopStyleColor();
                    
                    i++;
                };
                
                ImGui::EndChild();
            }
            
            ImGui::Text("hi!");
            ImGui::End();
        }
        else
        {
            selected = NULL;
        }
    }
    
    void Destroy()
    {
        
    }
    
    static SuperobjectInfoWindow* CreateWindow(const stSuperObject* target)
    {
        SuperobjectInfoWindow* window = new SuperobjectInfoWindow;
        window->selected = target;
        window->active = true;
        
        window_counter++;
        
        return window;
    }
};

static std::vector<struct SuperobjectInfoWindow*> superobject_info_windows;

struct list_param { int *selected, i; stSuperObject** so; stEngineObject** actor; stStandardGameInfo** stdgame; };

static void superobject_draw_childlist(void* data, void* p)
{
    stSuperObject* so = (stSuperObject*)data;
    if (!data) return;
    
    struct list_param* param = (struct list_param*)p;
    if (!param) return;
    
    const uint32_t type = host_byteorder_32(so->type);
    const char* name = "NULL";
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    
    switch (type)
    {
        case superobject_type_actor:
            const stEngineObject* actor = (const stEngineObject*)pointer(so->data);
            if (!actor) return;
            const stStandardGameInfo* stdgame = (const stStandardGameInfo*)pointer(actor->stdGame);
            if (!stdgame) return;
            name = fnActorGetName(actor_instance_name, actor, objectType);
            if (!name) name = fnActorGetName(actor_model_name, actor, objectType);
            color = ImColor(actor_color(actor));
            break;
    }
    
    
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    if (ImGui::Selectable(name, *param->selected == param->i))
    {
        *param->selected = param->i;
        *param->so = so;
        
        *param->actor = (stEngineObject*)pointer(so->data);
        if (!*param->actor) return;
        *param->stdgame = (stStandardGameInfo*)pointer((*param->actor)->stdGame);
        if (!*param->stdgame) return;
    }
    
    ImGui::PopStyleColor();
    
    param->i++;
    
    superobject_for_each_type(superobject_type_actor, (superobject_first_child(so)), objj) {
        superobject_draw_childlist(objj, param);
    }
}

static int selected = 0;
static bool just_selected = false;
static unsigned pc = 0;
//static IntCPA interpreter;
//static struct intcpa* interpreter;

bool vv = false;

static stSuperObject* selected_superobject = NULL;

void superobject_info(stSuperObject* so)
{
    if (!so) return;
    
    static bool is_open = true;
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Superobject info", &is_open))
    {
//        if (ImGui::BeginMenuBar())
//        {
//            if (ImGui::BeginMenu("File"))
//            {
//
//                ImGui::EndMenu();
//            }
//            ImGui::EndMenuBar();
//        }
        
        ImGui::BeginChild("SO List", ImVec2(200, 0), true);
        
        int i = 0;
        
        superobject_for_each_type(superobject_type_actor, so, object)
        {
            const stEngineObject* actor = (const stEngineObject*)superobject_data(object);
            if (!actor) continue;
            
            const stStandardGameInfo* stdgame = (const stStandardGameInfo*)pointer(actor->stdGame);
            if (!stdgame) return;
            
            /* Get actor instance name, or model name if spawnable actor */
            const char* name = fnActorGetName(actor_instance_name, actor, objectType);
            if (!name) name = fnActorGetName(actor_model_name, actor, objectType);
            if (!name) name = "Invalid object name";
            
            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
            uint32_t cc = actor_color(actor);
            if (cc) color = ImColor(cc);
            
            /* Draw the list item */
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            if (ImGui::Selectable(name, selected == i))
            {
                selected_superobject = object;
                selected = i;
                just_selected = true;
            }
            
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
            }
            
            ImGui::PopStyleColor();
            
            i++;
        };
        
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        
        stEngineObject* actor;
        stStandardGameInfo* stdgame;
        stBrain* brain;
        stMind* mind;
        stIntelligence* intelligence;
        stBehavior* behavior;
        stDsgMem* dsgmem;
        stDsgVar* vars;
        stTreeInterpret* script;
        
        static int line;
        
//        if (ImGui::Button("Pause CPU"))
//        {
//            extern void (*cpu_break)(void);
//            cpu_break();
//        }
        
        if (selected_superobject)
        {
            // Right
            ImGui::BeginGroup();
            ImGui::BeginChild("SO Info", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            
            if ((actor = (stEngineObject*)superobject_data(selected_superobject)))
            {
                if ((stdgame = (stStandardGameInfo*)pointer(actor->stdGame)))
                {
                    ImGui::BeginChild("Info", ImVec2(ImGui::GetContentRegionAvail().x, 250));
                    {
                        ImGui::BeginChild("Families", ImVec2(ImGui::GetContentRegionAvail().x, 25));
                        {
                            const char* family = fnActorGetName(actor_family_name, actor, objectType);
                            const char* model = fnActorGetName(actor_model_name, actor, objectType);
                            const char* instance = fnActorGetName(actor_instance_name, actor, objectType);
                            ImColor color1 = actor_color(actor);
                            ImColor color2 = color1;
                            ImColor color3 = color1;
                            color2.Value.w = 0.6f;
                            color3.Value.w = 0.45f;
                            
                            ImGui::TextColored(color1, "%s -", family);
                            ImGui::SameLine();
                            ImGui::TextColored(color2, "%s -", model);
                            ImGui::SameLine();
                            ImGui::TextColored(color3, "%s", instance);
                            
                            ImGui::EndChild();
                        }
                        
                        ImGui::BeginChild("A");
                        {
                            
                            ImGui::BeginChild("Flags", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), false);
                            {
                                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                                
                                ImGui::BeginChild("FlagName", ImVec2(130, 50));
//                                ImGui::TextColored(bit_off_color, "SPO flags");
//                                ImGui::TextColored(bit_off_color, "SPO draw flags");
//                                ImGui::TextColored(bit_off_color, "Custom bits");
//                                ImGui::TextColored(bit_off_color, "AI custom bits");
                                ImGui::EndChild();
                                
                                ImGui::SameLine();
                                
                                ImGui::BeginChild("FlagBits", ImVec2(ImGui::GetContentRegionAvail().x, 50));
//                                DisplayBits(&selected_superobject->flags, false, TABLE_LENGTH(R3SuperObjectFlagsTable), R3SuperObjectFlagsTable);
//                                DisplayBits(&selected_superobject->drawFlags, false);
//                                DisplayBits(&stdgame->customBits, true);
//                                DisplayBits(&stdgame->aiCustomBits, true, TABLE_LENGTH(R3AICustomBitsTable), R3AICustomBitsTable);
                                ImGui::EndChild();
                                
                                ImGui::PopStyleVar();
                            }
                            ImGui::EndChild();
                            
                            ImGui::SameLine();
                            
                            ImGui::BeginChild("Transform");
                            {
                                const stTransform* Tg = (const stTransform*)pointer(selected_superobject->globalTransform);
                                const stTransform* Tl = (const stTransform*)pointer(selected_superobject->localTransform);
//                                if (Tg) display_matrix4(matrix4_host_byteorder(Tg->matrix), ImVec4(1.0,1.0,1.0,1.0));
//                                if (Tl) display_matrix4(matrix4_host_byteorder(Tl->matrix), ImVec4(1.0,1.0,1.0,1.0));
                                
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndChild();
                }
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_Reorderable))
            {
//                if (ImGui::BeginTabItem("Object"))
//                {
//                    stTransform* transform = (stTransform*)pointer(selected_superobject->transform_global);
//                    stMatrix4D matrix = matrix4_from_internal(transform->matrix);
//                    DisplayMatrix4(matrix, ImVec4(1, 1, 1, 1));
//                    ImGui::EndTabItem();
//                }
                
                if (ImGui::BeginTabItem("AI"))
                {
                    //display_ai(actor);
                    
                    ImGui::EndTabItem();
                }
                
                
                const stDynam* dynam = actor_dynam(actor);
                if (dynam)
                {
                    if (ImGui::BeginTabItem("Dynamics"))
                    {
                        const stDynamics* dynamics = (const stDynamics*)pointer(dynam->dynamics);
                        //display_dynamics(dynamics);

                        ImGui::EndTabItem();
                    }
                }
                
//                if (ImGui::BeginTabItem("StdGame"))
//                {
//
//
//                    ImGui::EndTabItem();
//                }
                
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }
    }
    
    ImGui::End();
}

static void Draw()
{
    superobject_info(dynamic_world);
}

const Window SuperObjectWindow =
{
    Draw,
};
