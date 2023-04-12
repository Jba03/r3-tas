//
//  gui.cc
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include <math.h>
#include <string>

#include "gui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_memory_editor.h"

extern "C"
{
    #include "game.h"
    #include "stEngineStructure.h"
    #include "stEngineTimer.h"
    #include "stRandom.h"
    #include "configuration.h"
    #include "stVector3D.h"
    #include "stStandardGameInfo.h"
    #include "stSector.h"
    #include "graphics.h"
    #include "export.h"
    #include "stTreeInterpret.h"
    #include "stCollideSet.h"
    #include "geometry.h"
    #include "stDynamics.h"
    #include "stCameraGLI.h"
    #include "stMaterialGLI.h"
    #include "stTextureGLI.h"
    #include "xray.h"
}

/* ImGui */
extern ImGuiContext* GImGui;
static ImFont* font = NULL;
static ImVec2 display_size;

#pragma mark - Globals

static bool first = true;

static bool display_normals = false;
static bool view_rng_table = true;
static bool timer = true;

static bool menubar = true;

static bool view_general_info = true;
static bool view_game_input = true;
static bool view_input_structure = false;
static bool view_recording_tool = false;
static bool view_object_types = false;

static bool display_transition_counter = true;

static bool screen_projections_draw = false;
static bool screen_projections_ipos = false;
static bool screen_projections_use_cliprect = true;
static float screen_projections_bg_alpha = 0.1f;
static float screen_projections_overlay_alpha = 0.5f;
static float screen_projections_fade_factor = 0.5f;

static bool level_geometry_draw = false;
static bool level_geometry_draw_all_sectors = false;
static bool level_geometry_use_cliprect = true;
static float level_geometry_tri_fill = 0.75f;
static float level_geometry_fade_factor = 0.1f;

static MemoryEditor memory_viewer;
static struct xray xrayHandle;

extern "C" const char* (*get_config_path)(void);

#pragma mark - GUI

#include "type.cc"
#include "bits.cc"
#include "dsg.cc"
#include "ai.cc"

#include "dynamics.cc"
#include "superobject.cc"
#include "hierarchy.cc"
#include "rng.cc"
#include "joypad.cc"
#include "inputstructure.cc"
#include "recording-tool.cc"
#include "object-types.cc"
#include "geometry-display.cc"
#include "route.cc"

static void display_counters()
{
    const ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize;
    
    /* Draw counters */
    ImGui::Begin("Engine counters", &view_general_info, flags);
    {
        #define y(x) host_byteorder_32(*(engine->timer.counter + i * 4 + x))
        ImGui::SetWindowPos(ImVec2(display_size.x - 105, 45));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.25f), "F(%d,%d)", host_byteorder_32(engine->timer.frame), host_byteorder_32(engine->timer.usefulDeltaTime));
        for (unsigned int i = 0; i < 4; i++) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.25f), "%02d %02d %02d %02d", y(0), y(1), y(2), y(3));
        ImGui::End();
    }
    
    /* Draw level timer */
    ImGui::Begin("Timer", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
    {
        ImGui::SetWindowFontScale(2.0f);
        
        const int frames = host_byteorder_32(engine->timer.frame);
        const int ms = (int)(frames * (1000.0 / 60.0)) % 1000;
        const int s  = (frames / 60) % 60;
        const int m  = (frames / 60 / 60) % 60;
        const int h  = (frames / 60 / 60 / 60) % 24;
        ImGui::TextColored(ImVec4(1,1,1,0.25f), "%02d:%02d:%02d.%03d", h, m, s, ms);
        
        ImVec2 sz = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2(display_size.x - sz.x - 15, display_size.y - 50));
        ImGui::End();
    }
}

static void draw_rayman_info()
{
    if (actor_rayman)
    {
        const tdstDynam* dynam = actor_dynam(actor_rayman);
        if (!dynam) return;
        
        const tdstDynamics* dynamics = (const tdstDynamics*)pointer(dynam->dynamics);
        if (!dynamics) return;
        
        const tdstVector3D previousSpeed = vector3_host_byteorder(dynamics->base.previousSpeed);
        const float speed_x = previousSpeed.x;
        const float speed_y = previousSpeed.y;
        const float speed_h = sqrt(speed_x * speed_x + speed_y * speed_y);
        const float speed_z = previousSpeed.z;
        const float gravity = host_byteorder_f32(*(float32*)&dynamics->base.gravity);
        
        const tdstSuperObject* ray = (const tdstSuperObject*)actor_superobject(actor_rayman);
        if (!ray) return;
        
        const tdstTransform* T_ray = (const tdstTransform*)pointer(ray->globalTransform);
        if (T_ray)
        {
            const tdstMatrix4D M_ray = matrix4_host_byteorder(T_ray->matrix); // Player matrix
            const tdstVector3D P_ray = game_matrix4_position(M_ray); // Player position
            
            float time = (float)host_byteorder_32(engine->timer.deltaCount) / 1000.0f;
            
            /*  */
#define h(t,p) (-0.5f * gravity * (t * t) + (speed_z * t) + p)
            const float max_x = speed_z / gravity;
            const float max_height = h(max_x, P_ray.z);
            const float next_z = h(time, P_ray.z);
            
            /* Calculate X and Y displacement */
#define displacement(v,t) (((v + v) / 2.0f) * t)
            const float next_x = P_ray.x + displacement(speed_x, time);
            const float next_y = P_ray.y + displacement(speed_y, time);
            
            ImGui::Text("HSpeed = %.3f", speed_h);
            ImGui::Text("Speed = (%.2f, %.2f, %.2f)", speed_x, speed_y, speed_z);
            ImGui::Text("Pos = (%.2f, %.2f, %.2f)", P_ray.x, P_ray.y, P_ray.z);
            ImGui::Text("PosNext = (%.2f, %.2f, %.2f)", next_x, next_y, next_z);
            ImGui::Text("MaxJumpHeight = %.2f", max_height);
            ImGui::Text("Level end trigger (look) = %.3f°", degrees(fnActorHorizontalSightingRelative(actor_rayman, actor_changemap)));
            ImGui::Text("Level end trigger (trajectory) = %.3f°", degrees(fnActorTrajectoryAngleRelative(actor_rayman, actor_changemap)));
            
            
            ImDrawList* drawlist = ImGui::GetWindowDrawList();
            ImColor bg = ImColor(0.0f, 0.0f, 0.0f, 0.1f);
            draw_joystick(drawlist, ImVec2(0, ImGui::GetItemRectMin().y + 40), 25.0f, (M_ray.m00) * 50 * 2, (M_ray.m01) * 50 * 2, ImColor(1.0f, 0.0f, 0.0f), bg);
            draw_joystick(drawlist, ImVec2(0, ImGui::GetItemRectMin().y + 40), 25.0f, (M_ray.m01) * 50 * 2, (-M_ray.m00) * 50 * 2, ImColor(0.0f, 0.0f, 1.0f), bg);
            
            //            vector3* vector22 = NULL;
            //            if (actor_dsgvar(actor_rayman, 22, NULL, (void**)&vector22) >= 0)
            //            {
            //                vector3 v22 = vector3_host_byteorder(*vector22);
            //                v22.x -= M_ray.m01;
            //                v22.y -= M_ray.m00;
            //                v22 = vector3_normalize(v22);
            //
            //                draw_joystick(drawlist, ImVec2(55, ImGui::GetItemRectMin().y + 40), 25.0f, -v22.x * 50 * 2, v22.y * 50 * 2, ImColor(0.0f, 1.0f, 0.0f), bg);
            //            }
            
            ImGui::TextColored(ImColor(1.0f, 0.0f, 0.0f), "%.2f ", M_ray.m00);
            ImGui::SameLine();
            ImGui::TextColored(ImColor(0.0f, 0.0f, 1.0f), "%.2f", M_ray.m01);
            
#pragma mark Behavior
            {
                const char* behavior = NULL;
                if ((behavior = fnActorGetCurrentBehaviorName(actor_rayman)))
                {
                    if ((behavior = strrchr(behavior, ':')))
                    {
                        behavior += 1;
                        
                        /* Jump */
                        if (strcmp(behavior, "YAM_C_Saut1Impulsion") == 0
                            ||  strcmp(behavior, "YAM_C_Saut2Montee"   ) == 0
                            ||  strcmp(behavior, "YAM_C_Saut3Sommet"   ) == 0
                            ||  strcmp(behavior, "YAM_C_Saut4Reception") == 0)
                        {
                            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Jump()");
                        }
                        
                        //                        /* Helicopter */
                        //                        if (strcmp(behavior, "YAM_C_Helico") == 0)
                        //                        {
                        //                            const float next_speed = ai_function_temporal_real_combination(speed_z, 0.95f, -4.0f);
                        //
                        //                            int i = 0;
                        //                            float zspd = speed_z; /* Calculate approximate number of frames til maximum hover Z speed reached */
                        //                            while (zspd >= -3.99999f) { zspd = ai_function_temporal_real_combination(zspd, 0.95f, -4.0f); i++; }
                        //
                        //                            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Hover(%.3f, %d)", next_speed, i);
                        //                        }
                    }
                }
            }
            
            if (T_ray)
            {
                const tdstMatrix4D M_ray = matrix4_host_byteorder(T_ray->matrix); // Player matrix
                const tdstVector3D P_ray = game_matrix4_position(M_ray); // Player position
                const tdstSuperObject* current_sector = sector_by_location(father_sector, P_ray);
                if (current_sector)
                {
                    const tdstSector* sect = (const tdstSector*)superobject_data(current_sector);
                    if (sect) ImGui::Text("Current sector: %s", fnSectorGetName(sect));
                }
            }
        }
    }
}

static void draw_menubar()
{
    const ImGuiWindowFlags flags =
    ImGuiWindowFlags_MenuBar |
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize;
    
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.25f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::Begin("Main", &menubar, flags);
    {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
        ImGui::SetWindowSize(ImVec2(display_size.x, 0));
        ImGui::SetWindowPos(ImVec2(0, 0));
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
//                ImGui::Button("Export geometry");
//                if (ImGui::Button("Export scripts")) export_scripts();
//                ImGui::Separator();
                ImGui::Checkbox("Enable cheats?", &configuration.cheats.enabled);
                
                if (ImGui::Button("Dump memory")) game_memory_dump();
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::BeginMenu("Screen projections"))
                {
                    ImGui::Checkbox("Draw", &screen_projections_draw);
                    ImGui::Checkbox("Also draw IPOs?", &screen_projections_ipos);
                    ImGui::Checkbox("Clip to game viewport?", &screen_projections_use_cliprect);
                    ImGui::SliderFloat("Background alpha", &screen_projections_bg_alpha, 0.0f, 0.5f);
                    ImGui::SliderFloat("Overlay alpha", &screen_projections_overlay_alpha, 0.0f, 0.75f);
                    ImGui::SliderFloat("Distance fade factor", &screen_projections_fade_factor, 0.0f, 1.0f);
                    ImGui::EndMenu();
                }
                
                if (ImGui::BeginMenu("Level geometry"))
                {
                    ImGui::Checkbox("Draw", &level_geometry_draw);
                    ImGui::Checkbox("Draw all sectors", &level_geometry_draw_all_sectors);
                    ImGui::Checkbox("Clip to game viewport?", &level_geometry_use_cliprect);
                    ImGui::SliderFloat("Triangle base alpha", &level_geometry_tri_fill, 0.0f, 1.0f);
                    ImGui::SliderFloat("Distance fade factor", &level_geometry_fade_factor, 0.0f, 1.0f);
                    ImGui::EndMenu();
                }
                
                
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Display"))
            {
                ImGui::Checkbox("General info?", &view_general_info);
                ImGui::Checkbox("Superobject hierarchy?", &configuration.display.hierarchy);
                ImGui::Checkbox("Controller input?", &view_game_input);
                ImGui::Checkbox("Input structure?", &view_input_structure);
                ImGui::Checkbox("Object types?", &view_object_types);
                ImGui::Separator();
                ImGui::Checkbox("Memory viewer?", &memory_viewer.Open);
                
                if (configuration.cheats.enabled)
                    ImGui::Checkbox("Open recording tool?", &view_recording_tool);
                
                ImGui::EndMenu();
            }
            
            if (configuration.cheats.enabled)
            {
                if (ImGui::BeginMenu("Change map"))
                {
                    for (int i = 0; i < engine->levelCount; i++)
                    {
                        const char* level = engine->levelNames[i];
                        if (ImGui::MenuItem(level)) fnEngineLoadLevel(engine, level);
                    }
                    ImGui::EndMenu();
                }
            }
            
            
            //display_rng_info();
            
            ImGui::EndMenuBar();
        }
        
        ImGui::PopStyleVar();
        ImGui::End();
    }
    
    
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

static void draw_general_info()
{
    if (view_general_info)
    {
        ImGui::Begin("General info", &view_general_info, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
        {
            ImGui::SetWindowPos(ImVec2(0, 20));
            ImGui::SetWindowSize(ImVec2(300, 300));
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), configuration.cheats.enabled ? "CHEATS ENABLED" : "");
            
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
            draw_rayman_info();
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            
            ImGui::End();
        }
    }
}

void gui_render_game(void* xfb_texture)
{
    if (engine->mode == 0) return;
    
    if (first)
    {
        ImGui::SetNextWindowSize(ImVec2(648, 520));
    }
    
    printf("texture: %p\n", xfb_texture);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    //ImGui::SetNextWindowContentSize(ImVec2(648, 520));
    //ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), AspectRatio, NULL);
    ImGui::Begin("Game", &menubar, ImGuiWindowFlags_NoScrollbar | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoTitleBar);
    
    ImVec2 size = ImGui::GetWindowContentRegionMax();
    ImVec2 pos = ImVec2(display_size.x / 2.0f - size.x / 2.0f, display_size.y / 2.0f - size.y / 2.0f);
    if (first) { ImGui::SetWindowPos(pos); first = false; }
    
    //ImGui::Image(xfb_texture, size);
    ImGui::SetWindowSize(ImVec2(size.y * (648.0f / 520.0f), size.y));
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

static void gui_render_collision()
{
    if (first)
    {
        ImGui::SetNextWindowSize(ImVec2(648, 520));
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    //ImGui::SetNextWindowContentSize(ImVec2(648, 520));
    //ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), AspectRatio, NULL);
    ImGui::Begin("Game", &menubar, ImGuiWindowFlags_NoScrollbar | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoTitleBar);
    
    ImVec2 size = ImGui::GetWindowContentRegionMax();
    ImVec2 pos = ImVec2(display_size.x / 2.0f - size.x / 2.0f, display_size.y / 2.0f - size.y / 2.0f);
    if (first) { ImGui::SetWindowPos(pos); first = false; }
    
    //printf("texure: %p\n", graphics_get_texture());
    
    ImGui::Image(graphics_get_texture(), size);
    ImGui::SetWindowSize(ImVec2(size.y * (648.0f / 520.0f), size.y));
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

static void draw_hierarchy(ImVec2 pos)
{
    if (configuration.display.hierarchy)
    {
        ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize;
        
        ImGui::Begin("Superobject hierarchy", &menubar, flags);
        {
            ImGui::SetWindowPos(pos);
            ImGui::SetWindowSize(ImVec2(350, display_size.y - 300));
            ImGui::SetItemUsingMouseWheel();
            
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.6f), " ACTUAL WORLD");
            if (dynamic_world) display_hierarchy(dynamic_world, "Dynamic world");
            if (inactive_dynamic_world)  display_hierarchy(inactive_dynamic_world, "Inactive dynamic world");
            if (dynamic_world)  display_hierarchy(father_sector, "Father sector");
            
            ImGui::End();
        }
    }
}

static void draw_hierarchy_list(const tdstSuperObject* root)
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
    ImGui::BeginChild("SO List", ImVec2(200, 0), true, ImGuiWindowFlags_NoMove);
    //ImGui::SetWindowSize(size);
    
    int i = 0;
    superobject_for_each_type(superobject_type_actor, root, object)
    {
        const tdstEngineObject* actor = (const tdstEngineObject*)superobject_data(object);
        if (!actor) continue;
        
        const tdstStandardGameInfo* stdgame = (const tdstStandardGameInfo*)pointer(actor->stdGame);
        if (!stdgame) return;
        
        /* Get actor instance name, or model name if spawnable actor */
        const char* name = "NO!";//fnActorGetName(actor_instance_name, actor);
//        if (!name) name = fnActorGetName(actor_model_name, actor);
//        if (!name) name = "Invalid object name";
        
        ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
        uint32_t cc = actor_color(actor); //(uint32_t*)array_get(family_colors, host_byteorder_32(stdgame->family_type));
        color = ImColor(cc);
        
        /* Draw the list item */
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, color);
        if (ImGui::Selectable(name, selected == i))
        {
            
            //selected_superobject = object;
            selected = i;
            just_selected = true;
        }
        
        ImVec4 alt1 = ImVec4(1.0f, 1.0f, 1.0f, 0.050f);
        ImVec4 alt2 = ImVec4(1.0f, 1.0f, 1.0f, 0.075f);
        
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, ImColor((i % 2) == 0 ? alt1 : alt2));
        
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        
        i++;
    };
    
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

static void draw_hierarchy2(ImVec2 pos, ImVec2 size)
{
    static int current_item = 0;
    static const char* const items[] = {"Dynamic world", "Inactive dynamic world", "Father sector", "All three"};
    
    ImGui::Begin("SO List", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
    {
        ImGui::SetWindowPos(pos);
        ImGui::SetWindowSize(size);
        
        ImGui::Combo("", &current_item, items, 4);
        if (current_item == 0) draw_hierarchy_list(dynamic_world);
        if (current_item == 1) draw_hierarchy_list(inactive_dynamic_world);
        if (current_item == 2) draw_hierarchy_list(father_sector);
        if (current_item == 3)
        {
            draw_hierarchy_list(dynamic_world);
            draw_hierarchy_list(inactive_dynamic_world);
            draw_hierarchy_list(father_sector);
        }
        
        ImGui::End();
    }
}

extern "C" void gui_render_callback(void* ctx)
{
    GImGui = (ImGuiContext*)ctx;
    display_size = ImGui::GetIO().DisplaySize;
    
    if (first)
    {
        memory_viewer.Open = false;
        
        first = false;
    }
    
//    ImGui::Begin("XRAY");
//    if (ImGui::Button("Initialize"))
//    {
//        memset(xrayHandle.nodes, 0, 200000 * sizeof(struct xray_node));
//        memset(xrayHandle.pointset, 0, 65536 * 4 * sizeof(tdstVector3D));
//        xrayHandle.n_points = 0;
//        xrayHandle.n_nodes = 0;
//        xrayInitialize(&xrayHandle);
//    }
//    ImGui::End();
    
    const float ar = display_size.x / display_size.y;
    const float new_width = display_size.y * (640.0f / 528.0f) * 1.075f;
    const float x_offset = display_size.x - new_width;
    ImGui::SetWindowPos(ImVec2(x_offset / 2.0f, 0));
    ImGui::SetWindowSize(ImVec2(new_width, display_size.y));
    
    ImVec2 off = ImGui::GetWindowPos();
    ImVec2 sz = ImGui::GetWindowSize();
    ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
        
    const tdstVector3D playerposition = game_matrix4_position(fnActorGetMatrix(actor_rayman));
//    const tdstVector3D speed = vector3_add(playerposition, actor_speed(actor_rayman));
//
//    ImVec4 r = project_world_coordinate(playerposition);
//    ImVec4 spd = project_world_coordinate(speed);
//    ImVec2 rpos = ImVec2(off.x + sz.x - r.x * sz.x, off.y + r.y * sz.y);
//    ImVec2 spos = ImVec2(off.x + sz.x - spd.x * sz.x, off.y + spd.y * sz.y);
//
//    drawlist->AddLine(rpos, spos, ImColor(1.0f, 0.0f, 0.0f, 1.0f));
    
    if (engine->mode == 5)
    {
        const ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize;
        
        ImGui::Begin("Transition counter", &display_transition_counter, flags);
        ImGui::TextColored(ImColor(1.0f, 0.6f, 0.0f), "TRANSITION: %d", transition_frame);
        
        ImVec2 windowpos = display_size;
        windowpos.x /= 2.0f;
        windowpos.y /= 2.0f;
        
        ImVec2 windowsize = ImGui::GetWindowSize();
        ImGui::SetWindowSize(ImVec2(120, 20));
        windowpos.x -= windowsize.x / 2.0f;
        windowpos.y -= windowsize.y / 2.0f;
        
        ImGui::SetWindowPos(windowpos);
        ImGui::End();
    }
    
    ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoBackground;
    
    ImGui::Begin("RNG Table", &view_rng_table, flags);
    ImGui::SetWindowSize(ImVec2(150, display_size.y));
    display_rng_table();
    ImGui::SetWindowPos(ImVec2(display_size.x - ImGui::GetWindowWidth(), 140));
    ImGui::End();
    
    if (engine)
    {
        display_counters();
    }
    
    if (hierarchy)
    {
        superobject_info(dynamic_world);
        
        /* Draw superobject windows */
        //for (SuperobjectInfoWindow* window : superobject_info_windows) window->Draw();
        
        if (screen_projections_draw) ProjectSuperobjectPositions(dynamic_world);
        if (screen_projections_ipos) ProjectSuperobjectPositions(father_sector);
        
        const tdstSuperObject* sector = sector_by_location(father_sector, fnActorGetPosition(actor_rayman));
        if (level_geometry_draw) DrawLevelGeometry(level_geometry_draw_all_sectors ? father_sector : sector);
        
    }
    
//    if (xrayHandle.path[0] != NULL)
//    {
//        for (int i = 0; i < 1000; i++)
//        {
//            struct xray_node* node1 = xrayHandle.path[i + 0];
//            struct xray_node* node2 = xrayHandle.path[i + 1];
//            if (node1 && node2)
//            {
//                ImVec4 projected1 = project_world_coordinate(node1->position);
//                ImVec4 projected2 = project_world_coordinate(node2->position);
//
//                ImVec2 screen1 = ImVec2(off.x + sz.x - projected1.x * sz.x, off.y + projected1.y * sz.y);
//                ImVec2 screen2 = ImVec2(off.x + sz.x - projected2.x * sz.x, off.y + projected2.y * sz.y);
//
//                //if (screen.x >= 0 && screen.x < display_size.x && screen.y >= 0 && screen.y < display_size.y && projected.z > 0)
//                //drawlist->AddNgon(screen, 10.0f, ImColor(10.0f, 0.0, 0.0f, 1.0f), 3);
//                if (projected1.w < 0 && projected2.w < 0 && projected1.z < 0 && projected2.z < 0) {} else
//                    drawlist->AddLine(screen1, screen2, ImColor(10.0f, 0.0, 0.0f, 1.0f));
//            }
//        }
//
//
//
        //printf("node: %d\n", xray.n_nodes);
//        for (int i = 0; i < xrayHandle.n_nodes; i++)
//        {
//            struct xray_node node1 = xrayHandle.nodes[i];
//            {
//                ImVec4 projected1 = project_world_coordinate(node1.position);
//                ImVec2 screen = ImVec2(off.x + sz.x - projected1.x * sz.x, off.y + projected1.y * sz.y);
//
//                if (screen.x >= 0 && screen.x < display_size.x && screen.y >= 0 && screen.y < display_size.y && projected1.z > 0)
//                {
//                    drawlist->AddNgonFilled(screen, 3.0f, ImColor(10.0f, 0.0, 0.0f, 1.0f), 3);
//                    //drawlist->AddText(screen, ImColor(0.0f, 1.0f, 0.0f), std::to_string(i).c_str());
//                }
//
//            }
//        }
//    }
    
    //DisplayRouteWindow();
    
    //for (int i = 0; i < n_routes; i++) DrawRoute(routes[i], off, sz, drawlist);
    
//    #pragma mark IPO closest vertex
//    {
//        extern tdstVector3D sphere_pos;
//        extern int n_triangles;
//        extern struct triangle* triangles;
//        
//        ImGui::SetNextWindowPos(ImVec2(400,50));
//        ImGui::Begin("Calculate nearest point");
//        tdstSuperObject* soo;
//        const tdstInstantiatedPhysicalObject* ipo = ipo_find("IPO:IPO_w_e_clairiere", father_sector, &soo);
//        ImGui::Text("IPO: %X\n", offset(ipo));
//        static tdstVector3D pointa = {};
//        const tdstMatrix4D T = fnActorGetMatrix(actor_rayman);
//        if (actor_rayman)
//        {
//            const struct tdstPhysicalObject* po = (const struct tdstPhysicalObject*)pointer(ipo->tdstPhysicalObject);
//            if (!po) goto skip;
//            
//            const tdstPhysicalCollideSet* collset = (const tdstPhysicalCollideSet*)pointer(po->physical_collideset);
//            if (!collset) goto skip;
//            
//            const tdstCollideObject* zdr = (const tdstCollideObject*)pointer(collset->zdr);
//            if (!zdr) goto skip;
//            
//            //n_triangles = collide_object_triangles_combined(zdr, matrix4_identity, &triangles);
//            
//            collide_object_closest_vertex_to(zdr, fnSuperobjectGetGlobalMatrix(soo), game_matrix4_position(T), NULL, NULL, &pointa);
//            sphere_pos = pointa;
//        }
//    skip:
//        ImGui::Text("%.3f %.3f %.3f", pointa.x, pointa.y, pointa.z);
//        ImGui::End();
//    }
    
//    ImGui::Begin("a");
//    //if (ImGui::Button("Check visibility"))
//    {
//        extern bool line_of_sight(struct xray *h, struct xray_node *a, struct xray_node *b);
//
//        extern float st[OCTREE_MAX_SELECTED_NODES];
//        extern tdstOctreeNode* selected_nodes[OCTREE_MAX_SELECTED_NODES];
//        extern int n_selected_nodes;
//
//        n_selected_nodes = 0;
//        memset(st, 0, sizeof(float) * OCTREE_MAX_SELECTED_NODES);
//        memset(selected_nodes, 0, sizeof(tdstOctreeNode*) * OCTREE_MAX_SELECTED_NODES);
//
//        struct xray_node a;
//        a.position = actor_position(actor_rayman);
//        a.sector = (tdstSuperObject*)sector_by_location(father_sector, a.position);
//
//        struct xray_node b;
//        b.position = actor_position(actor_find(actor_instance_name, "BEN_Box2", dynamic_world));
//        b.sector = (tdstSuperObject*)sector_by_location(father_sector, b.position);
//
//        ImGui::Text("Line of sight: %s", line_of_sight(&xray, &a, &b) ? "true" : "false");
//    }
//    ImGui::End();
    
    draw_menubar();
    draw_general_info();
    
    if (hierarchy) draw_hierarchy(ImVec2(0, ImGui::GetWindowPos().y + 250));
    
    
    display_joypad(&view_game_input);
    display_recording_tool(&view_recording_tool);
    display_input_structure(&view_input_structure);
    display_object_types(&view_object_types);
    
    /* Draw memory viewer */
    memory_viewer.ReadOnly = !configuration.cheats.enabled;
    std::string fmt = "Memory (" + std::string(memory_viewer.ReadOnly ? "readonly" : "writeable") + ")";
    if (memory_viewer.Open) memory_viewer.DrawWindow(fmt.c_str(), (void*)memory.base, 24 * 1000 * 1000);
}
