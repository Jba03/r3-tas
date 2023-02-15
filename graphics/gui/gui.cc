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
//#include "vector3.h"
#include "vector2.h"
#include "stdgame.h"
#include "sector.h"
#include "graphics.h"
#include "export.h"
}


extern ImGuiContext* GImGui;
static ImFont* font = NULL;
static ImVec2 display_size;

static bool first = true;

static bool general_info = true;
static bool display_normals = false;
static bool view_rng_table = true;
static bool timer = true;

static bool view_game_input = true;
static bool view_input_structure = false;
static bool view_recording_tool = false;

static bool display_transition_counter = true;

static MemoryEditor memory_viewer;
static bool display_memory_viewer = true;


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

static void display_counters()
{
    const ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize;
    
    ImGui::Begin("Engine counters", &general_info, flags);
    ImGui::SetWindowPos(ImVec2(display_size.x - 105, 45));
    ImGui::SetWindowFontScale(1);
    
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "Counters");
    
    struct engine_timer timer = engine->timer;
    #define y(x) host_byteorder_32(*(timer.counter + i * 4 + x))
    for (unsigned int i = 0; i < 4; i++)
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.25f), "%02d %02d %02d %02d", y(0), y(1), y(2), y(3));
    
    ImGui::End();
}

static void draw_endtrigger_angle()
{
    if (hierarchy)
    {
        const struct superobject* ray = (const struct superobject*)actor_superobject(actor_rayman);
        const struct superobject* nin = (const struct superobject*)actor_superobject(actor_changemap);
        if (ray && nin)
        {
            const struct transform* T_ray = (const struct transform*)pointer(ray->transform_global);
            const struct transform* T_nin = (const struct transform*)pointer(nin->transform_global);
            
            const matrix4 M_ray = matrix4_host_byteorder(T_ray->matrix);
            const matrix4 M_nin = matrix4_host_byteorder(T_nin->matrix);
            
            const vector3 P_ray = game_matrix4_position(M_ray);
            const vector3 P_nin = game_matrix4_position(M_nin);
            const float ninrel = atan2(P_nin.y - P_ray.y, P_nin.x - P_ray.x);
            const float raylook = atan2(M_ray.m00, M_ray.m01);
            
            float angle = degrees(raylook + ninrel);
            if (angle >= 180.0f) angle = 360.0f - angle;
            
            ImGui::Text("Level end trigger: %.3f°", fabs(angle));
        }
    }
}

static void draw_general_info()
{
    const ImGuiWindowFlags flags =
    ImGuiWindowFlags_MenuBar |
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize;
    
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    
    ImGui::Begin("Main", &general_info, flags);
    ImGui::SetWindowSize(ImVec2(display_size.x, 0));
    ImGui::SetWindowPos(ImVec2(0, 0));
    
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Button("Export geometry");
            if (ImGui::Button("Export scripts")) export_scripts();
            ImGui::Separator();
            ImGui::Checkbox("Enable cheats?", &configuration.cheats.enabled);
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View"))
        {
            ImGui::Checkbox("Display superobject hierarchy?", &configuration.display.hierarchy);
            ImGui::Checkbox("Display controller input?", &view_game_input);
            ImGui::Checkbox("Display input structure?", &view_input_structure);
            ImGui::Separator();
            ImGui::Checkbox("Open memory viewer?", &memory_viewer.Open);
            
            if (configuration.cheats.enabled)
                ImGui::Checkbox("Open recording tool?", &view_recording_tool);
            
            ImGui::EndMenu();
        }
        
        if (configuration.cheats.enabled)
        {
            if (ImGui::BeginMenu("Change map"))
            {
                for (int i = 0; i < array_element_count(level_names); i++)
                {
                    const char* level = (const char*)array_get(level_names, i);
                    if (ImGui::MenuItem(level))
                        level_load(level);
                }
                ImGui::EndMenu();
            }
        }
        
        ImGui::Text("|");
        
        draw_endtrigger_angle();
        
        
        //display_rng_info();
        
        ImGui::EndMenuBar();
    }
    
    ImGui::End();
    
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

static void move_camera()
{
//    if (ImGui::IsKeyDown('W')) camera->position = struct vector3_add(camera->position, struct vector3_mulf(camera->front, camera->speed));
//    if (ImGui::IsKeyDown('A')) camera->position = struct vector3_sub(camera->position, struct vector3_mulf(camera->right, camera->speed));
//    if (ImGui::IsKeyDown('S')) camera->position = struct vector3_sub(camera->position, struct vector3_mulf(camera->front, camera->speed));
//    if (ImGui::IsKeyDown('D')) camera->position = struct vector3_add(camera->position, struct vector3_mulf(camera->right, camera->speed));
//
//    ImVec2 mouse = ImGui::GetMousePos();
//
//    ImVec2 relative = ImVec2(mouse.x - prev_mouse.x, prev_mouse.y - mouse.y);
//    camera_update(camera, relative.y, relative.x, false);
//
    //prev_mouse = mouse;
}

static void display_transform(struct transform* transform)
{
    matrix4 mat = matrix4_host_byteorder(transform->matrix);
    
#define f32 host_byteorder_f32
    ImGui::Text("%.2f  %.2f  %.2f  %.2f", mat.m00, mat.m01, mat.m02, mat.m03);
    ImGui::Text("%.2f  %.2f  %.2f  %.2f", mat.m10, mat.m11, mat.m12, mat.m13);
    ImGui::Text("%.2f  %.2f  %.2f  %.2f", mat.m20, mat.m21, mat.m22, mat.m23);
    ImGui::Text("%.2f  %.2f  %.2f  %.2f", mat.m30, mat.m31, mat.m32, mat.m33);
#undef f32
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
    ImGui::Begin("Game", &general_info, ImGuiWindowFlags_NoScrollbar | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoTitleBar);
    
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
    ImGui::Begin("Game", &general_info, ImGuiWindowFlags_NoScrollbar | /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoTitleBar);
    
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

extern "C" void gui_render_callback(void* ctx)
{
    GImGui = (ImGuiContext*)ctx;
    
    if (first)
    {
    }
    
    display_size = ImGui::GetIO().DisplaySize;
    
    //gui_render_collision();
    
    draw_general_info();
    //draw_timer();
    //draw_rayman_position();
    
    
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
    
    if (configuration.cheats.camera_unlocked)
        move_camera();
    
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
        if (hierarchy->n_children > 0)
        {
            struct superobject* dynamic_world = (struct superobject*)pointer(hierarchy->first_child);
            struct superobject* inactive_dynamic_world = (struct superobject*)pointer(dynamic_world->next);
            struct superobject* father_sector = (struct superobject*)pointer(hierarchy->last_child);
            
            superobject_info(dynamic_world);
            
            if (configuration.display.hierarchy)
            {
                flags |= ImGuiWindowFlags_NoBackground;
                flags &= ~ImGuiWindowFlags_NoScrollWithMouse;
                ImGui::Begin("Superobject hierarchy", &general_info, flags);
                ImGui::SetWindowPos(ImVec2(0, 50));
                ImGui::SetWindowSize(ImVec2(350, display_size.y - 200));
                ImGui::SetItemUsingMouseWheel();
                
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.6f), " ACTUAL WORLD");
                display_hierarchy(dynamic_world, "Dynamic world");
                display_hierarchy(inactive_dynamic_world, "Inactive dynamic world");
                display_hierarchy(father_sector, "Father sector");
                
                ImGui::End();
            }
        }
    }
    
    display_joypad(&view_game_input);
    display_recording_tool(&view_recording_tool);
    display_input_structure(&view_input_structure);
    
    /* Draw memory viewer */
    memory_viewer.ReadOnly = !configuration.cheats.enabled;
    std::string fmt = "Memory (" + std::string(memory_viewer.ReadOnly ? "readonly" : "writeable") + ")";
    if (memory_viewer.Open) memory_viewer.DrawWindow(fmt.c_str(), (void*)memory.base, 24 * 1000 * 1000);
}
