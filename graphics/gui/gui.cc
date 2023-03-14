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

/* Should sum up to 1, I guess */
static float projX = 0.377f;
static float projY = 0.708f;

static ImVec4 project_world_coordinate(const struct vector3 P)
{
    const struct gli_camera* camera = (const struct gli_camera*)pointer(engine->viewport_camera[0]);
    
    struct matrix4 view = matrix4_host_byteorder(camera->transform.matrix);
    /* Change sign of the two middle columns (flipping the rotation) */
    view.m01 = -view.m01;
    view.m11 = -view.m11;
    view.m21 = -view.m21;
    view.m31 = -view.m31;
    view.m02 = -view.m02;
    view.m12 = -view.m12;
    view.m22 = -view.m22;
    view.m32 = -view.m32;
    /* const struct matrix4 view = matrix4_lookat(campos, vector3_host_byteorder(*(struct vector3*)(memory.base + 0x00c53910)) (dsgvar_150), vector3_new(0, 0, -1)); */
    
    const float fov = host_byteorder_f32(*(float32*)&camera->alpha_x);
    
    const struct matrix4 projection = matrix4_perspective(fov, 528.0f / 640.0f, 0.1f, 1000.0f);
    const struct matrix4 viewprojection = matrix4_mul(projection, view);
    const struct vector4 P2 = vector4_new(P.x, P.y, P.z, 1.0f);
    const struct vector4 R = vector4_mul_matrix4(P2, viewprojection);
    
    float xp = (R.x / R.w) * projX + 0.5f;
    float yp = (R.y / R.w) * projY + 0.5f;
    
    return ImVec4(xp, yp, R.z, R.w);
}

static ImVec4 project_world_coordinate(const struct vector4 P)
{
    return project_world_coordinate(vector3_new(P.x, P.y, P.z));
}

static void RecursiveProjectSuperobjectPosition(const struct superobject* object,
                                                const struct matrix4 transform,
                                                const struct vector3 player,
                                                const float fov,
                                                ImVec2 offset,
                                                ImVec2 size,
                                                ImVec2 mouse,
                                                ImDrawList* drawlist)
{
    if (!object) return;
    
    const struct matrix4 T = matrix4_mul(superobject_matrix_global(object), transform);
    
    const char* text = superobject_name(object);
    if (text)
    {
        const struct vector3 P = game_matrix4_position(T);
        /* Project the object's position to the screen */
        ImVec4 projected = project_world_coordinate(P);
        /* Only draw objects in view */
        if (projected.w >= 0)
        {
            
            ImVec2 pos = ImVec2(offset.x + size.x - projected.x * size.x, offset.y + projected.y * size.y);
            ImVec2 text_pos = ImVec2(pos.x - 1, pos.y);
            ImVec2 text_extent = ImVec2(pos.x + strlen(text) * 7, pos.y + 13);
            
            /* Fade out objects far away */
            const float alpha = 1.0f / vector3_length(vector3_sub(P, player));
            
            ImColor color = ImColor(1.0f, 1.0f, 1.0f, 0.25f);
            ImColor background = color;
            
            if (superobject_type(object) == superobject_type_ipo)
            {
                background.Value.w = screen_projections_bg_alpha;
            }
            
            if (superobject_type(object) == superobject_type_actor)
            {
                background = color = ImColor(actor_color((struct actor*)superobject_data(object)));
                color.Value.w = alpha * ((1.0f - screen_projections_fade_factor) * 200.0f) / (fov / 2.0f);
                background.Value.w = screen_projections_bg_alpha;
            }
            
            if (mouse.x >= pos.x && mouse.y >= pos.y && mouse.x <= text_extent.x && mouse.y <= text_extent.y)
            {
                ImGui::BeginTooltip();
                ImGui::TextColored(color, "%s", text);
                ImGui::Text("(%.3f, %.3f, %.3f)", P.x, P.y, P.z);
                ImGui::Text("Click to view");
                ImGui::EndTooltip();
                
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_superobject = (struct superobject*)object;
                    
                    bool already_open = false;
                    for (SuperobjectInfoWindow* window : superobject_info_windows)
                        if (window->selected == object) already_open = true;
                    
                    if (!already_open) superobject_info_windows.push_back(SuperobjectInfoWindow::CreateWindow(object));
                }
                
                color = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
            }
            
            /* Draw label */
            drawlist->AddCircleFilled(text_pos, 1.0f, color);
            drawlist->AddRectFilled(text_pos, text_extent, background);
            drawlist->AddText(pos, color, text);
        }
    }
    
    superobject_for_each(object, child) RecursiveProjectSuperobjectPosition(child, T, player, fov, offset, size, mouse, drawlist);
}

static void ProjectSuperobjectPositions(const struct superobject* root)
{
    float fov = host_byteorder_f32(*(float32*)(memory.base + 0x00C751B4));
    fov = fov == 0.0f ? 1.3 : fov;
    
    const float ar = display_size.x / display_size.y;
    const float new_width = display_size.y * (640.0f / 528.0f) * 1.075f;
    const float x_offset = display_size.x - new_width;
    ImGui::SetWindowPos(ImVec2(x_offset / 2.0f, 0));
    ImGui::SetWindowSize(ImVec2(new_width, display_size.y));
    
    const struct vector3 playerposition = game_matrix4_position(actor_matrix(actor_rayman));
    
    ImVec2 off = ImGui::GetWindowPos();
    ImVec2 sz = ImGui::GetWindowSize();
    ImVec2 cursor = ImGui::GetMousePos();
    ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
    
    if (screen_projections_use_cliprect)
        drawlist->PushClipRect(ImVec2(x_offset / 2.0f, 0), ImVec2(new_width + x_offset / 2.0f, display_size.y));
    
    /* Overlay to make seeing the text easier */
    drawlist->AddRectFilled(off, ImVec2(sz.x + off.x, display_size.y), ImColor(ImVec4(0.0f, 0.0f, 0.0f, screen_projections_overlay_alpha)));
    
    RecursiveProjectSuperobjectPosition(root, matrix4_identity, playerposition, fov, off, sz, cursor, drawlist);
    
    if (screen_projections_use_cliprect)
        drawlist->PopClipRect();
}

static void DrawLevelGeometryRecursive(const struct superobject* root, const struct matrix4 transform, const struct vector3 player, ImVec2 off, ImVec2 sz, ImDrawList* drawlist, ImVec4 color = ImColor(1.0f, 0.75f, 0.0f, 1.0f))
{
    if (!root) return;
    
    /* Calculate the new transformation */
    const struct matrix4 T = matrix4_mul(superobject_matrix_global(root), transform);

    if (root == viewed_sector) color = ImColor(0.0f, 1.0f, 0.25f, 1.0f);
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const struct ipo* ipo = (const struct ipo*)superobject_data(root);
        if (ipo)
        {
            const struct collide_object* zdr = ipo_collide_object(ipo);
            if (zdr)
            {
                int mesh_idx = 0;
                const struct collide_mesh* mesh;
                while ((mesh = collide_object_mesh(zdr, mesh_idx)))
                {
                    const uint16* indices = (const uint16*)pointer(mesh->face_indices);
                    const struct vector3* vertices = (const struct vector3*)pointer(zdr->vertices);
                    
                    for (int16 index = 0; index < host_byteorder_16(mesh->n_faces); index++)
                    {
                        uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
                        uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
                        uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));
                        
                        const struct vector3 a = vector3_host_byteorder(*(vertices + idx0));
                        const struct vector3 b = vector3_host_byteorder(*(vertices + idx1));
                        const struct vector3 c = vector3_host_byteorder(*(vertices + idx2));
                        
                        const struct vector4 A = vector4_mul_matrix4(vector4_new(a.x, a.y, a.z, 1.0f), T);
                        const struct vector4 B = vector4_mul_matrix4(vector4_new(b.x, b.y, b.z, 1.0f), T);
                        const struct vector4 C = vector4_mul_matrix4(vector4_new(c.x, c.y, c.z, 1.0f), T);
                        
                        ImVec4 coord_a = project_world_coordinate(A);
                        ImVec4 coord_b = project_world_coordinate(B);
                        ImVec4 coord_c = project_world_coordinate(C);
                        
                        if (coord_a.w >= 0 && coord_b.w >= 0 && coord_c.w >= 0)
                        {
                            /* Calculate the triangle center for highlighting of the face */
                            const struct vector3 center = vector3_new((A.x + B.x + C.x) / 3.0f, (A.y + B.y + C.y) / 3.0f, (A.z + B.z + C.z) / 3.0f);
                            /* Distance fading factor */
                            const float alpha = 1.0f / (vector3_length(vector3_sub(center, player)) + 1.0f);
                            
                            ImVec2 pos1 = ImVec2(off.x + sz.x - coord_a.x * sz.x, off.y + coord_a.y * sz.y);
                            ImVec2 pos2 = ImVec2(off.x + sz.x - coord_b.x * sz.x, off.y + coord_b.y * sz.y);
                            ImVec2 pos3 = ImVec2(off.x + sz.x - coord_c.x * sz.x, off.y + coord_c.y * sz.y);
                            
                            ImColor outline = color;
                            outline.Value.w = alpha * (1.0f - level_geometry_fade_factor) + level_geometry_fade_factor;
                            
                            ImColor base = outline;
                            base.Value.w *= level_geometry_tri_fill;
                            
                            drawlist->AddTriangle(pos1, pos2, pos3, outline);
                            drawlist->AddTriangleFilled(pos1, pos2, pos3, base);
                        }
                    }
                    
                    mesh_idx++;
                }
            }
        }
    }
    
    /* Draw children */
    superobject_for_each(root, child) DrawLevelGeometryRecursive(child, T, player, off, sz, drawlist, color);
}

static void DrawLevelGeometry(const struct superobject* sector)
{
    const float ar = display_size.x / display_size.y;
    const float new_width = display_size.y * (640.0f / 528.0f) * 1.075f;
    const float x_offset = display_size.x - new_width;
    ImGui::SetWindowPos(ImVec2(x_offset / 2.0f, 0));
    ImGui::SetWindowSize(ImVec2(new_width, display_size.y));
    
    ImVec2 off = ImGui::GetWindowPos();
    ImVec2 sz = ImGui::GetWindowSize();
    ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
    
    if (level_geometry_use_cliprect)
        drawlist->PushClipRect(ImVec2(x_offset / 2.0f, 0), ImVec2(new_width + x_offset / 2.0f, display_size.y));
        
    const struct vector3 playerposition = game_matrix4_position(actor_matrix(actor_rayman));
//    const struct vector3 speed = vector3_add(playerposition, actor_speed(actor_rayman));
//
//    ImVec4 r = project_world_coordinate(playerposition);
//    ImVec4 spd = project_world_coordinate(speed);
//    ImVec2 rpos = ImVec2(off.x + sz.x - r.x * sz.x, off.y + r.y * sz.y);
//    ImVec2 spos = ImVec2(off.x + sz.x - spd.x * sz.x, off.y + spd.y * sz.y);
//
//    drawlist->AddLine(rpos, spos, ImColor(1.0f, 0.0f, 0.0f, 1.0f));
    
    DrawLevelGeometryRecursive(sector, matrix4_identity, playerposition, off, sz, drawlist);
    
    if (level_geometry_use_cliprect)
        drawlist->PopClipRect();
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
    
    ImGui::Begin("a");
    ImGui::SliderFloat("Proj X", &projX, 0.0f, 1.0f);
    ImGui::SliderFloat("Proj Y", &projY, 0.0f, 1.0f);
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
    
//    
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
