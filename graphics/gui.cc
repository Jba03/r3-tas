//
//  gui.cpp
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

extern "C"
{
#include "gui.h"
#include "engine.h"
#include "actor.h"
#include "common.h"
#include "configuration.h"
}

#include "imgui.h"

extern struct Engine *engine;


extern ImGuiContext* GImGui;

static ImVec2 display_size;

static bool general_info = true;
static bool display_normals = false;
static bool timer = true;

static void draw_timer()
{
    const ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoResize;
    
    ImGui::Begin("Timer", &general_info, flags);
    ImGui::SetWindowPos(ImVec2(display_size.x - 150, display_size.y - 45));
    ImGui::SetWindowFontScale(1);
    
    ImGui::TextColored(ImVec4(1.0, 1.0, 1.0, 0.5), "00:45.341");
    
    ImGui::End();
}

static void draw_matrix(struct Matrix4 m)
{
    ImGui::Begin("Matrix", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::TextColored(ImVec4(0.0, 0.75, 0.45, 1.0), "Matrix display");
    ImGui::TextColored(ImVec4(0.0, 0.75, 0.45, 1.0), "%.2f  %.2f  %.2f  %.2f", m.row0.x, m.row0.y, m.row0.z, m.row0.w);
    ImGui::TextColored(ImVec4(0.0, 0.75, 0.45, 1.0), "%.2f  %.2f  %.2f  %.2f", m.row1.x, m.row1.y, m.row1.z, m.row1.w);
    ImGui::TextColored(ImVec4(0.0, 0.75, 0.45, 1.0), "%.2f  %.2f  %.2f  %.2f", m.row2.x, m.row2.y, m.row2.z, m.row2.w);
    ImGui::TextColored(ImVec4(0.0, 0.75, 0.45, 1.0), "%.2f  %.2f  %.2f  %.2f", m.row3.x, m.row3.y, m.row3.z, m.row3.w);
    ImGui::End();
}

static void draw_superobject_tree(struct SuperObject *so)
{
    ImGui::PushID(so);
    const char* typename2 = superobject_typename(so);
    const char* name = superobject_typename(so);
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,1));
    
    if (so->type == SUPEROBJECT_TYPE_ACTOR)
    {
        struct Actor* actor = (struct Actor*)so->data;
        if (actor)
        {
            if (actor->instance_name)
            {
                //info("%s", actor->instance_name);
                name = actor->instance_name;
            }
            
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, ImU32(family_color_table[actor->info->type.family]));
//            name = actor->instance_name;
//            info("%s", name);
        }
    }
    
    
    if (ImGui::TreeNodeEx(typename2, ImGuiTreeNodeFlags_FramePadding, "%s", name))
    {
//        if (ImGui::SliderFloat("X", &so->matrix_default->row3->x, -400.0, 400.0))
//            i_writefloat(so->matrix_default->row3->x, so->matrix_default->row3->offset);
//
//        if (ImGui::SliderFloat("Z", &so->matrix_default->row3->z, -400.0, 400.0))
//            i_writefloat(so->matrix_default->row3->z, so->matrix_default->row3->offset + 4);
//
//        if (ImGui::SliderFloat("Y", &so->matrix_default->row3->y, -400.0, 400.0))
//            i_writefloat(so->matrix_default->row3->y, so->matrix_default->row3->offset + 8);
        
        for (int i = 0; i < so->n_children; i++)
        {
            draw_superobject_tree(so->children[i]);
        }
        
        ImGui::TreePop();
    }
    
    ImGui::PopID();
    ImGui::PopStyleColor();
}

static void draw_hierarchy(struct SuperObject* so)
{
    const ImGuiWindowFlags flags =
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_AlwaysAutoResize;
    
    ImGui::Begin("Superobject hierarchy", NULL, flags);
    ImGui::SetWindowPos(ImVec2(0, 20));
    draw_superobject_tree(so);
    //ImGui::TreePop();
    ImGui::End();
}

static void draw_general_info()
{
    display_size = ImGui::GetIO().DisplaySize;
    
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
        ImGui::Button("Reload");
        
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::Checkbox("Display normals", &display_normals);
            ImGui::Checkbox("Unlock camera?", &configuration.camera_unlocked);
            ImGui::Checkbox("Enable cheats", &configuration.enable_cheats);
            configuration.graphics_display_mode = display_normals ? 1 : 0;
            
            ImGui::EndMenu();
        }
        
        if (configuration.enable_cheats)
        {
            if (ImGui::BeginMenu("Change map"))
            {
                for (int i = 0; i < engine->level_count; i++)
                {
                    const char* level = engine->level_names[i];
                    if (ImGui::MenuItem(level))
                        engine_change_map(engine, level);
                }
                ImGui::EndMenu();
            }
        }
        
        ImGui::SameLine(ImGui::GetWindowWidth() - 400);
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "Hello!");
        
        ImGui::EndMenuBar();
    }
    
    
    ImGui::End();
    
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    
}

static void draw_rayman_position()
{
    struct Vector3 rayman = vector3_read(0x00BF0D98);
    ImGui::Begin("Rayman");
    ImGui::Text("X: %f", rayman.x);
    ImGui::Text("Y: %f", rayman.y);
    ImGui::Text("Z: %f", rayman.z);
    ImGui::End();
}

ImVec2 prev_mouse;

static void move_camera()
{
    if (ImGui::IsKeyDown('W')) camera->position = vector3_add(camera->position, vector3_mulf(camera->front, camera->speed));
    if (ImGui::IsKeyDown('A')) camera->position = vector3_sub(camera->position, vector3_mulf(camera->right, camera->speed));
    if (ImGui::IsKeyDown('S')) camera->position = vector3_sub(camera->position, vector3_mulf(camera->front, camera->speed));
    if (ImGui::IsKeyDown('D')) camera->position = vector3_add(camera->position, vector3_mulf(camera->right, camera->speed));
    
    ImVec2 mouse = ImGui::GetMousePos();
    
    ImVec2 relative = ImVec2(mouse.x - prev_mouse.x, prev_mouse.y - mouse.y);
    camera_update(camera, relative.y, relative.x, false);
    
    prev_mouse = mouse;
}

void render_callback(void* ctx)
{
    GImGui = (ImGuiContext*)ctx;
    
    draw_general_info();
    draw_timer();
    draw_rayman_position();
    
    if (configuration.camera_unlocked)
        move_camera();
    
//    if (engine)
//    {
//        if (engine->root)
//        {
//            struct Actor* rayman = actor_find("Rayman", engine->root);
//            if (rayman)
//            {
//                rayman->superobject->matrix_default = matrix4_read(rayman->superobject->matrix_default->offset);
//                draw_matrix(rayman->superobject->matrix_default);
//            }
//            
//            draw_hierarchy(engine->root);
//        }
//    }
}
