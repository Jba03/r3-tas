//
//  superobject.cpp
//  r3-tas
//
//  Created by Jba03 on 2022-12-22.
//

#include <bitset>

#include "actor.h"
#include "brain.h"
#include "mind.h"
#include "intelligence.h"
#include "behavior.h"
#include "script.h"
#include "dsg.h"
#include "transform.h"
#include "aimodel.h"

extern "C" {
    #include "translate.h"
    #include "interpret.h"
    #include "input.h"
    #include "intfun.h"
}

static int window_counter = 0;

struct SuperobjectInfoWindow
{
    bool active;
    bool sidebar;
    int sidebar_idx;
    const struct superobject* selected;
    
    SuperobjectInfoWindow()
    {
        active = true;
        sidebar = false;
    }
    
    void Draw()
    {
        if (active && hierarchy)
        {
            const char* name = superobject_name(selected);
            if (!name) name = "Invalid object name";
            
            ImGui::Begin(name);
            
            if (true)
            {
                ImGui::BeginChild("SO List", ImVec2(200, 0), true);
                
                int i = 0;
                
                const struct superobject* world = dynamic_world;
                if (superobject_type(selected) == superobject_type_actor) world = dynamic_world;
                if (superobject_type(selected) == superobject_type_ipo) world = father_sector;
                if (superobject_type(selected) == superobject_type_sector) world = father_sector;
                
                superobject_for_each(world, child)
                {
                    const struct actor* actor = (const struct actor*)superobject_data(child);
                    if (!actor) continue;
                    
                    /* Get actor instance name, or model name if spawnable actor */
                    const char* name = superobject_name(child);
                    if (!name) name = "Invalid object name";
                    
                    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
                    if (superobject_type(child) == superobject_type_actor) color = ImColor(actor_color(actor));
                    
                    /* Draw the list item */
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    if (ImGui::Selectable(name, sidebar_idx == i))
                    {
                        selected = (struct superobject*)child;
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
    
    static SuperobjectInfoWindow* CreateWindow(const struct superobject* target)
    {
        SuperobjectInfoWindow* window = new SuperobjectInfoWindow;
        window->selected = target;
        window->active = true;
        
        window_counter++;
        
        return window;
    }
};

static std::vector<SuperobjectInfoWindow*> superobject_info_windows;

struct list_param { int *selected, i; struct superobject** so; struct actor** actor; struct standard_game_info** stdgame; };

static void superobject_draw_childlist(void* data, void* p)
{
    struct superobject* so = (struct superobject*)data;
    if (!data) return;
    
    struct list_param* param = (struct list_param*)p;
    if (!param) return;
    
    const uint32_t type = host_byteorder_32(so->type);
    const char* name = "NULL";
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    
    switch (type)
    {
        case superobject_type_actor:
            const struct actor* actor = (const struct actor*)pointer(so->data);
            if (!actor) return;
            const struct standard_game_info* stdgame = (const struct standard_game_info*)pointer(actor->stdgame);
            if (!stdgame) return;
            name = actor_name(actor_instance_name, actor);
            if (!name) name = actor_name(actor_model_name, actor);
            color = ImColor(actor_color(actor));
            break;
    }
    
    
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    if (ImGui::Selectable(name, *param->selected == param->i))
    {
        *param->selected = param->i;
        *param->so = so;
        
        *param->actor = (struct actor*)pointer(so->data);
        if (!*param->actor) return;
        *param->stdgame = (struct standard_game_info*)pointer((*param->actor)->stdgame);
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

static std::vector<std::string> superobject_flag_description =
{
    "Not pickable",
    "Hidden",
    "No transformation matrix",
    "Zoom instead of scale",
    "Type of bounding volume",
    "Superimposed",
    "Ignore raycasts",
    "No shadow",
    "Semilookat",
    "CheckChildren",
    "Render on near plane",
};

static std::vector<std::string> ai_custom_bits_description =
{
    "Unseen Frozen Anim Player",
    "Need Module Matrices",
    "Movable",
    "Projectile",
    "RayTrace Hit",
    "Targetable",
    "Cannot crush Principal Actor",
    "Pickable",
    "ActorHasAShadow",
    "ShadowOnMe (10)",
    "Prunable (11)",
    "Out Of visibility [internal use]",
    "Unseen Frozen",
    "No Anim Player",
    "Fightable",
    "No Meca",
    "No AI",
    "Destroy when Anim ended",
    "No Anim Player When Too Far",
    "No AI Player When Too Far",
    "Unfreezable",
    "Uses Transparency Zone",
    "No Meca When Too Far",
    "Sound When Invisible",
    "CustomBit_25",
    "Camera Hit",
    "CanPushPrincipalActor",
    "Designer bit 1",
    "Designer bit 2",
    "Designer bit 3",
    "Designer bit 4",
    "PrincipalActor",
};

static struct superobject* selected_superobject = NULL;

void superobject_info(struct superobject* so)
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
            const struct actor* actor = (const struct actor*)superobject_data(object);
            if (!actor) continue;
            
            const struct standard_game_info* stdgame = (const struct standard_game_info*)pointer(actor->stdgame);
            if (!stdgame) return;
            
            /* Get actor instance name, or model name if spawnable actor */
            const char* name = actor_name(actor_instance_name, actor);
            if (!name) name = actor_name(actor_model_name, actor);
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
            ImGui::PopStyleColor();
            
            i++;
        };
        
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        
        struct actor* actor;
        struct standard_game_info* stdgame;
        struct brain* brain;
        struct mind* mind;
        struct intelligence* intelligence;
        struct behavior* behavior;
        struct dsgmem* dsgmem;
        struct dsgvar* vars;
        struct script* script;
        
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
            
            if ((actor = (struct actor*)superobject_data(selected_superobject)))
            {
                if ((stdgame = (struct standard_game_info*)pointer(actor->stdgame)))
                {
                    ImGui::BeginChild("Info", ImVec2(ImGui::GetContentRegionAvail().x, 250));
                    {
                        ImGui::BeginChild("Families", ImVec2(ImGui::GetContentRegionAvail().x, 25));
                        {
                            const char* family = actor_name(actor_family_name, actor);
                            const char* model = actor_name(actor_model_name, actor);
                            const char* instance = actor_name(actor_instance_name, actor);
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
                                ImGui::TextColored(bit_off_color, "SPO flags");
                                ImGui::TextColored(bit_off_color, "SPO draw flags");
                                ImGui::TextColored(bit_off_color, "Custom bits");
                                ImGui::TextColored(bit_off_color, "AI custom bits");
                                ImGui::EndChild();
                                
                                ImGui::SameLine();
                                
                                ImGui::BeginChild("FlagBits", ImVec2(ImGui::GetContentRegionAvail().x, 50));
                                DisplayBits(&selected_superobject->flags, false, superobject_flag_description);
                                DisplayBits(&selected_superobject->draw_flags, false);
                                DisplayBits(&stdgame->custom_bits, true);
                                DisplayBits(&stdgame->ai_custom_bits, true, ai_custom_bits_description);
                                ImGui::EndChild();
                                
                                ImGui::PopStyleVar();
                            }
                            ImGui::EndChild();
                            
                            ImGui::SameLine();
                            
                            ImGui::BeginChild("Transform");
                            {
                                const struct transform* Tg = (const struct transform*)pointer(selected_superobject->transform_global);
                                const struct transform* Tl = (const struct transform*)pointer(selected_superobject->transform_local);
                                if (Tg) display_matrix4(matrix4_host_byteorder(Tg->matrix), ImVec4(1.0,1.0,1.0,1.0));
                                if (Tl) display_matrix4(matrix4_host_byteorder(Tl->matrix), ImVec4(1.0,1.0,1.0,1.0));
                                
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
//                    struct transform* transform = (struct transform*)pointer(selected_superobject->transform_global);
//                    struct matrix4 matrix = matrix4_from_internal(transform->matrix);
//                    DisplayMatrix4(matrix, ImVec4(1, 1, 1, 1));
//                    ImGui::EndTabItem();
//                }
                
                
                const struct dynam* dynam = actor_dynam(actor);
                if (dynam)
                {
                    if (ImGui::BeginTabItem("AI"))
                    {
                        display_ai(actor);
                        
                        ImGui::EndTabItem();
                    }
                    
                    if (ImGui::BeginTabItem("Dynamics"))
                    {
                        const struct dynamics* dynamics = (const struct dynamics*)pointer(dynam->dynamics);
                        display_dynamics(dynamics);

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
