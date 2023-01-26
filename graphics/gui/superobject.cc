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
            color = ImColor(*(uint32_t*)array_get(family_colors, host_byteorder_32(stdgame->family_type)));
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

static int frame = 0;


void superobject_info(struct superobject* so)
{
    frame++;
    
    if (!so) return;
    
    static bool is_open = true;
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Superobject info", &is_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        ImGui::BeginChild("SO List", ImVec2(200, 0), true);
        
        int i = 0;
        static struct superobject* selected_superobject = NULL;
        superobject_for_each_type(superobject_type_actor, so, object)
        {
            const struct actor* actor = (const struct actor*)superobject_data(object);
            if (!actor) continue;
            
            const struct standard_game_info* stdgame = (const struct standard_game_info*)pointer(actor->stdgame);
            if (!stdgame) return;
            
            /* Get actor instance name, or model name if spawnable actor */
            const char* name = actor_name(actor_instance_name, actor);
            if (!name) name = actor_name(actor_model_name, actor);
            
            /* Get actor family color */
            ImVec4 color = ImColor(*(uint32_t*)array_get(family_colors, host_byteorder_32(stdgame->family_type)));
            
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
                ImGui::Text("Father: %X\n", offset(superobject_parent(selected_superobject)));
                
                if ((stdgame = (struct standard_game_info*)pointer(actor->stdgame)))
                {
                    ImGui::BeginChild("Flags", ImVec2(ImGui::GetContentRegionAvail().x, 60), false);
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
                    if (ImGui::BeginTabItem("Dynamics"))
                    {
                        const struct dynamics* dynamics = (const struct dynamics*)pointer(dynam->dynamics);
                        display_dynamics(dynamics);
                        
                        ImGui::EndTabItem();
                    }
                }
                
                
                if (ImGui::BeginTabItem("AI"))
                {
                    display_ai(actor);
                    
                    ImGui::EndTabItem();
                }
                
                if (ImGui::BeginTabItem("StdGame"))
                {
                    
                    
                    ImGui::EndTabItem();
                }
                
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
//            if (ImGui::Button("Open in memory viewer")) memory_viewer.GotoAddrAndHighlight(so_offset, so_offset + 4);
//            ImGui::SameLine();
//            if (ImGui::Button("Save")) {}
            ImGui::EndGroup();
        }
    }
    
    ImGui::End();
}
