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
    #include "engine.h"
    #include "engine_timer.h"
    #include "rnd.h"
    #include "configuration.h"
    #include "vector3.h"
    #include "vector2.h"
    #include "stdgame.h"
    #include "sector.h"
    #include "graphics.h"
    #include "export.h"
    #include "script.h"
    #include "collideset.h"
    #include "geometry.h"
    #include "dynamics.h"
    #include "camera.h"
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

static bool screen_projections_draw = true;
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
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.25f), "F(%d,%d)", host_byteorder_32(engine->timer.frame), host_byteorder_32(engine->timer.delta_time_useful));
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
        const struct dynam* dynam = actor_dynam(actor_rayman);
        if (!dynam) return;
        
        const struct dynamics* dynamics = (const struct dynamics*)pointer(dynam->dynamics);
        if (!dynamics) return;
        
        const vector3 previousSpeed = vector3_host_byteorder(dynamics->base.speed_previous);
        const float speed_x = previousSpeed.x;
        const float speed_y = previousSpeed.y;
        const float speed_h = sqrt(speed_x * speed_x + speed_y * speed_y);
        const float speed_z = previousSpeed.z;
        const float gravity = host_byteorder_f32(*(float32*)&dynamics->base.gravity);
        
        const struct superobject* ray = (const struct superobject*)actor_superobject(actor_rayman);
        if (!ray) return;
        
        const struct transform* T_ray = (const struct transform*)pointer(ray->transform_global);
        if (T_ray)
        {
            const matrix4 M_ray = matrix4_host_byteorder(T_ray->matrix); // Player matrix
            const vector3 P_ray = game_matrix4_position(M_ray); // Player position
            
            float time = (float)host_byteorder_32(engine->timer.timer_count_delta) / 1000.0f;
            
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
            ImGui::Text("Level end trigger (look) = %.3f°", degrees(actor_horizontal_sighting_relative(actor_rayman, actor_changemap)));
            ImGui::Text("Level end trigger (trajectory) = %.3f°", degrees(actor_trajectory_angle_relative(actor_rayman, actor_changemap)));
            
            
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
                if ((behavior = actor_current_behavior_name(actor_rayman)))
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
                const matrix4 M_ray = matrix4_host_byteorder(T_ray->matrix); // Player matrix
                const vector3 P_ray = game_matrix4_position(M_ray); // Player position
                const struct superobject* current_sector = sector_by_location(father_sector, P_ray);
                if (current_sector)
                {
                    const struct sector* sect = (const struct sector*)superobject_data(current_sector);
                    if (sect) ImGui::Text("Current sector: %s", sector_name(sect));
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
                    for (int i = 0; i < engine->level_count; i++)
                    {
                        const char* level = engine->level_names[i];
                        if (ImGui::MenuItem(level)) engine_load_level(engine, level);
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

static void draw_hierarchy_list(const struct superobject* root)
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
    ImGui::BeginChild("SO List", ImVec2(200, 0), true, ImGuiWindowFlags_NoMove);
    //ImGui::SetWindowSize(size);
    
    int i = 0;
    superobject_for_each_type(superobject_type_actor, root, object)
    {
        const struct actor* actor = (const struct actor*)superobject_data(object);
        if (!actor) continue;
        
        const struct standard_game_info* stdgame = (const struct standard_game_info*)pointer(actor->stdgame);
        if (!stdgame) return;
        
        /* Get actor instance name, or model name if spawnable actor */
        const char* name = "NO!";//actor_name(actor_instance_name, actor);
//        if (!name) name = actor_name(actor_model_name, actor);
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
        
        const struct superobject* sector = sector_by_location(father_sector, actor_position(actor_rayman));
        if (level_geometry_draw) DrawLevelGeometry(level_geometry_draw_all_sectors ? father_sector : sector);
        
    }
    
//    #pragma mark IPO closest vertex
//    {
//        extern struct vector3 sphere_pos;
//        extern int n_triangles;
//        extern struct triangle* triangles;
//        
//        ImGui::SetNextWindowPos(ImVec2(400,50));
//        ImGui::Begin("Calculate nearest point");
//        struct superobject* soo;
//        const struct ipo* ipo = ipo_find("IPO:IPO_w_e_clairiere", father_sector, &soo);
//        ImGui::Text("IPO: %X\n", offset(ipo));
//        static struct vector3 pointa = {};
//        const struct matrix4 T = actor_matrix(actor_rayman);
//        if (actor_rayman)
//        {
//            const struct physical_object* po = (const struct physical_object*)pointer(ipo->physical_object);
//            if (!po) goto skip;
//            
//            const struct physical_collideset* collset = (const struct physical_collideset*)pointer(po->physical_collideset);
//            if (!collset) goto skip;
//            
//            const struct collide_object* zdr = (const struct collide_object*)pointer(collset->zdr);
//            if (!zdr) goto skip;
//            
//            //n_triangles = collide_object_triangles_combined(zdr, matrix4_identity, &triangles);
//            
//            collide_object_closest_vertex_to(zdr, superobject_matrix_global(soo), game_matrix4_position(T), NULL, NULL, &pointa);
//            sphere_pos = pointa;
//        }
//    skip:
//        ImGui::Text("%.3f %.3f %.3f", pointa.x, pointa.y, pointa.z);
//        ImGui::End();
//    }
    
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
