//
//  configuration.cpp
//  r3-tas
//
//  Created by Jba03 on 2023-07-17.
//

#include "gui.hh"

static int Layout = 0;

static bool HierarchyChanges = false;

static const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

static void DrawHierarchySettings(ImVec2 position) {
    ImGui::SetNextWindowPos(position);
    ImGui::Begin("Hierarchy", nullptr, WindowFlags);
    ImGui::Checkbox("View changes?", &HierarchyChanges);
    ImGui::Separator();
    
    ImGui::End();
}

static void DrawLayoutSettings(ImVec2 position) {
    ImGui::SetNextWindowPos(position);
    ImGui::Begin("Layout preset", nullptr, WindowFlags);
    ImGui::RadioButton("Practice", &Layout, 0);
    ImGui::RadioButton("Tool Assisted Speedrunning", &Layout, 1);
    ImGui::End();
}

static void Draw() {
    if (ConfigurationWindow.Open) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        ImGui::Begin("Configuration", &ConfigurationWindow.Open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
        
        if (ImGui::IsWindowFocused()) {
            ImGui::SetWindowFocus("Hierarchy");
        }
        
        ImGui::End();
        ImGui::PopStyleVar();
        
        DrawHierarchySettings(ImVec2(100, 200));
        DrawLayoutSettings(ImVec2(100, 100));
    }
}

Window ConfigurationWindow = { &Draw, NULL, false };
