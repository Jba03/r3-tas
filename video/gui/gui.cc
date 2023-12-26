//
//  gui.cc
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-07-06.
//

#include "game.hh"
#include "gui.hh"
#include "graphics.hh"

#include "imgui_memory_editor.h"

#include <iostream>

/* Global ImGUI context */
extern ImGuiContext* GImGui;
static bool DrawGame = true;
static float TimeScale = 1.0f;

using namespace game;

static void DrawGameWindow(ImTextureID T, bool *windowed)
{
    MemoryEditor editor;
    editor.ReadOnly = false;
    editor.DrawWindow("A", (void*)memoryBase, 24 * 1000 * 1000);
    
    *windowed = p_stActualWorld != NULL;
    
    if (DrawGame && p_stActualWorld)
    {
        *windowed = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        
        ImGui::SetNextWindowContentSize(ImVec2(640, 528));
        ImGui::Begin("Game", &DrawGame, ImGuiWindowFlags_MenuBar);
        ImVec2 WindowPos = ImGui::GetWindowPos();
        ImVec2 WindowSize = ImGui::GetWindowSize();
        
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        
        if (ImGui::BeginMenuBar())
        {
            ImGui::TextColored(ImColor(1.0f, 1.0f, 1.0f, 0.5f), "%s", g_stEngineStructure->engineFrozen ? "M" : std::to_string(g_stEngineStructure->mode).c_str());
            
            
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            if (ImGui::BeginMenu(g_stEngineStructure->currentLevelName))
            {
                ImGui::PopStyleColor();

                ImGui::ResetMouseDragDelta();
                for (uint8 i = 0; i < g_stEngineStructure->levelCount; i++)
                {
                    std::string LevelName = g_stEngineStructure->levelNames[i];
                    bool selected = g_stEngineStructure->currentLevelName == LevelName;
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, i % 2 ? 1.0f : 0.75f));
                    if (ImGui::MenuItem(LevelName.c_str(), NULL, &selected))
                        g_stEngineStructure->loadLevel(LevelName);
                    
                    ImGui::PopStyleColor();
                }

                ImGui::EndMenu();
            }
            else
            {
                ImGui::PopStyleColor();
            }

            //ImGui::TextColored(ImColor(1.0f, 0.6f, 0.0f), "Transit: %d", transition_frame);

            ImGui::EndMenuBar();
        }
        
        ImGui::Image(T, ImGui::GetWindowSize());
        ImGui::End();
        
        static bool noclip = false;
        
        ImGui::Begin("Game window extras", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos(ImVec2(WindowPos.x, WindowPos.y + WindowSize.y + 10));
        ImGui::SetWindowSize(ImVec2(WindowSize.x, 0));
        ImGui::Checkbox("Noclip/ghost mode", reinterpret_cast<bool*>(game::GST_g_bGhostMode));
        ImGui::SliderFloat("Engine timescale", &TimeScale, 0.1f, 2.0f);
        //g_stEngineStructure->timer.ticksPerMs.data = game_byteorder_32(uint32_t(40500 * 1.0f / TimeScale));
        ImGui::End();
        
        if (IPT_g_stInputStructure && p_stDynamicWorld && !g_stEngineStructure->engineFrozen)
        {
            if (IPT_g_stInputStructure->entries[IPT_E_Entry_Button_Jump].state + 1 > 0)
            {
                //stSuperObject *rayman = p_stDynamicWorld->find("Rayman", game::g_stObjectTypes);
//                //rayman->IPO;
//
//                //stDynamics *dynamics = ((stDynam*)object->dynam)->dynamics;
//
//                //std::cout << "name: " << object->getName(actor_family_name, objectType) << "\n";
//                //printf("jump: %X\n", IPT_g_stInputStructure->entries[IPT_E_Entry_Button_Jump].state.offset());
//                //rayman->localTransform->matrix.m30 = game_byteorder_f32(1.1);//vector3_new(1.0f, 1.0f, 1.0f);
//               // printf("jump! %X\n", rayman->data.offset());
            }
        }
        
        //if (*(bool*)(memoryBase + GCN_POINTER_NOCLIP_MODE))
        {
            //uint32_t v = IPT_g_stInputStructure->numEntries;
            //printf("%X\n", swap32(v));
            //printf("aa\n");
            
//
//            for (unsigned int i = 0; i < IPT_g_stInputStructure->numEntries; i++)
//            {
//                stInputEntryElement element = IPT_g_stInputStructure->entries[i];//.offsetByType<stInputEntryElement>(3);
//                std::cout << "#define IPT_E_Entry_" << std::hex << std::string(element.actionName) << " " + std::to_string(i) << "\n";
//            }
//
//            printf("\n\n\n");
                
            
            //std::cout << std::hex << std::to_string(element.state) << "\n";
//            if (element.state > 0)
//            {
//                printf("aaa\n");
//            }
            
            
//            if (!(input.button.a->state & 0xFF000000))
//            {
//                printf("fly\n");
//                //stDynamics *dyn = fnActorGetDynamics(actor_rayman);
//                //dyn->base.currentTransform.scale = vector4_new(0.5, 0.5f, 0.5f, 1.0f);
//            }
        }
    }
    else
    {
        *windowed = false;
    }
}

namespace gui
{
    
    void initialize(void)
    {
    }
    
    void draw(void *c, void *texture, bool *windowed) {
        GImGui = (ImGuiContext*)c;
        DrawGameWindow(texture, windowed);
        
        //graphics::beginFrame(640, 528);
        
        if (!ConfigurationWindow.Open) {
            MenuBar.Draw();
            RNGWindow.Draw();
            HierarchyWindow.Draw();
            RunCreateWindow.Draw();
            TextureViewerWindow.Draw();
        }
        
        ConfigurationWindow.Draw();
        //    MovieInputWindow.Draw();
        //    ScriptWindow.Draw();
        //    SuperObjectWindow.Draw();
    }
    
}
