//
//  menubar.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-07-17.
//

#include "gui.hh"
#include "game.hh"

static bool CheatsEnabled = false;

static void Draw() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Configuration")) ConfigurationWindow.Open = true;
        ImGui::Separator();
        ImGui::Checkbox("Enable cheats", &CheatsEnabled);
        
        ImGui::EndMenu();
    }
    
    if (ImGui::BeginMenu("Level")) {
        if (ImGui::BeginMenu("Reload")) {
            game::g_stEngineStructure->mode = 6;
        }
            
        if (ImGui::BeginMenu("Change")) {
            for (uint8 i = 0; i < game::g_stEngineStructure->levelCount; i++) {
                std::string LevelName = game::g_stEngineStructure->levelNames[i];
                bool selected = game::g_stEngineStructure->currentLevelName == LevelName;
                if (ImGui::MenuItem(LevelName.c_str(), NULL, &selected))
                    game::g_stEngineStructure->loadLevel(LevelName);
            }
            ImGui::EndMenu();
        }
      
        ImGui::Separator();
        
        if (ImGui::MenuItem("New run")) {
            RunCreateWindow.Open = true;
        }
        
        ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
}


Window MenuBar = { &Draw };
